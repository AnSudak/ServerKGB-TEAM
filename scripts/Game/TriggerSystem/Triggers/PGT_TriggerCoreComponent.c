[ComponentEditorProps(category: "PGT/Triggers/init", description: "Trigger Component")]
class PGT_TriggerCoreComponentClass : ScriptComponentClass
{
}

class PGT_TriggerCoreComponent : ScriptComponent
{
	[Attribute(defvalue: "500", desc: "Радиус триггера", category: "Main")]
	protected int m_Radius;
	
	[Attribute(defvalue: "10", desc: "Период проверки", category: "Main")]
	protected int m_CheckPeriod;
	
	[Attribute(defvalue: "1", desc: "Заспавнить после старта статичиские группы", category: "Main")]
	protected bool m_afterStartSpawnStatic;
	
	[Attribute(defvalue: "1", desc: "Заспавнить после старта динамические группы", category: "Main")]
	protected bool m_afterStartSpawnDynamic;
	
	[Attribute(desc: "Подкрепления", category: "Reinforcements")]
	ref array<ref PGT_BaseReinformContainer> m_aReinforcements;
	
	protected ref array<IEntity> m_listOfDynamicGroups;
	protected ref array<IEntity> m_listOfOnceGroups;
	protected ref array<IEntity> m_aNearbyCharacters;

	protected PGT_TriggerComponent m_trigger;
	protected SCR_ScenarioFrameworkLayerTask m_task
	protected float m_fCheckDelay;
	
	int m_ticksDelay;
	int m_ticks;
	int m_Index;
	
	//------------------------------------------------------------------------------------------------
	void StartTrigger(IEntity owner)
	{
		m_Index = 0;
		m_ticks = 0;
		m_ticksDelay = 0;
				
		Print("PGT: Trigger Started - " + GetOwner().GetName());
		
		m_trigger.ActivateTask();
		
		if (m_afterStartSpawnStatic)
			SpawnOnceGroups();
		
		if (m_afterStartSpawnDynamic)
			SpawnDynamicGroups();
		
		GetGame().GetCallqueue().CallLater(StartEFrame, 20000, false, owner);
	}	
	
	
	override void EOnFrame(IEntity owner, float timeSlice)
	{
		super.EOnFrame(owner, timeSlice);
		vector ownerPos = owner.GetOrigin();
		
		m_fCheckDelay -= timeSlice;
		if (m_fCheckDelay <= 0)
		{			
			m_fCheckDelay = m_CheckPeriod;
			m_aNearbyCharacters.Clear();
			
			owner.GetWorld().QueryEntitiesBySphere(ownerPos, m_Radius, QueryEntitiesCallbackMethod, null, EQueryEntitiesFlags.DYNAMIC | EQueryEntitiesFlags.WITH_OBJECT);
			int FiaCount = m_aNearbyCharacters.Count();
		
			Print("PGT: Core cycle, units avaliable - " + FiaCount);
			
			if (m_Index >= 0) {
				if (m_ticksDelay > 0) {
					m_ticks++;
					if (m_ticks >= m_ticksDelay){
						m_ticksDelay = 0;
						m_ticks = 0;				
					} else return;
				}
				
				//==================================================================
				foreach (PGT_BaseReinformContainer action : m_aReinforcements)
				{
					if (action.GetIndex() != m_Index)
						continue;
					
					Print("PGT: Try request action = " + action.Type() + " == Index: " + action.GetIndex() + " == CountAI: " + action.GetCountAI() + " == IgnoreAI: " + action.GetIgnoreAI() );
					
					if (!action.GetIgnoreAI() && FiaCount > action.GetCountAI())
						continue;
					
					action.Init(GetOwner());
					m_ticksDelay = action.GetTicks();
					m_Index++;
					break;
				}
			} else {
				if (!m_task) {
					Print("PGT: Task is null...");
					return;
				}
				
				if (FiaCount < 8)
				{
					SCR_ScenarioFrameworkTaskSupportEntity helper = m_task.m_SupportEntity;
					helper.FinishTask(m_task.m_Task);
					ClearEventMask(owner, EntityEvent.FRAME);
					Print("PGT: Core is stopped");
				}
			}
		}
	}
	
	
	protected bool QueryEntitiesCallbackMethod(IEntity e)
	{
		if (!e) // only ai
			return false;
		
		if (SCR_ChimeraCharacter.Cast(e)) {
			SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(e);
			if (character.GetFactionKey() == "FIA") {
				SCR_DamageManagerComponent damageManager = SCR_DamageManagerComponent.GetDamageManager(character);
				if (damageManager && damageManager.GetState() != EDamageState.DESTROYED)
					m_aNearbyCharacters.Insert(e);
			}
		}

		return true;
	}
	
	//------------------------------------------------------------------------------------------------
	void RegisterGroup(IEntity entity, bool m_Dynamic)
	{
		if (m_Dynamic)
			m_listOfDynamicGroups.Insert(entity);
		else 
			m_listOfOnceGroups.Insert(entity);
		
		Print("PGT: Group registered - " + entity.GetName() + " --- Dynamic: " + m_Dynamic);
	}
	
	void SpawnOnceGroups()
	{
		foreach (IEntity group : m_listOfOnceGroups)
		{
			PGT_GroupComponent g_comp = PGT_GroupComponent.GetThis(group);
			g_comp.SpawnGroup();
		}
	}
	
	void SpawnDynamicGroups()
	{
		foreach (IEntity group : m_listOfDynamicGroups)
		{
			PGT_GroupComponent g_comp = PGT_GroupComponent.GetThis(group);
			g_comp.SpawnGroup();
		}
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void OnPostInit(IEntity owner)
	{
		if (SCR_Global.IsEditMode())
			return;
		
		m_trigger = PGT_TriggerComponent.GetThis(owner);
		
		m_listOfDynamicGroups = {};
		m_listOfOnceGroups = {};
		m_aNearbyCharacters = {};
		 
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	void SetEndState(SCR_ScenarioFrameworkLayerTask task)
	{
		m_task = task;
		m_Index = -1;
	}
	
	void StartEFrame(IEntity owner)
	{
		SetEventMask(owner, EntityEvent.FRAME);
	}
	
	static PGT_TriggerCoreComponent GetThis(IEntity owner)
	{
	    PGT_TriggerCoreComponent component = PGT_TriggerCoreComponent.Cast(owner.FindComponent(PGT_TriggerCoreComponent));
	    if (component == null)
	    {
	        Print("PGT: PGT_TriggerCoreComponent not found.");
	        return null;
	    }
    
    	return component;
	}
}