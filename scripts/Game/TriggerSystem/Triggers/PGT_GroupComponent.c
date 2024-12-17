[ComponentEditorProps(category: "PGT/Triggers/groups", description: "Trigger Component")]
class PGT_GroupComponentClass : ScriptComponentClass
{
}

class PGT_GroupComponent : ScriptComponent
{
	[Attribute(defvalue: "", desc: "Группа", category: "Main")]
	ResourceName m_GroupPrefab;
	
	[Attribute(defvalue: "0", desc: "Будет использоватся для одного раза или динамическо спавна", category: "Dynamic")]
	bool m_Dynamic;
	
	[Attribute(defvalue: "20", desc: "Радиус проверки на наличие человека", category: "Dynamic")]
	int m_Radius;

	[Attribute(defvalue: EAISkill.REGULAR.ToString(), UIWidgets.ComboBox, "AI skill in combat", "", ParamEnumArray.FromEnum(EAISkill), category: "Common")]
	EAISkill m_eAISkill;

	[Attribute(defvalue: EAICombatType.NORMAL.ToString(), UIWidgets.ComboBox, "AI combat type", "", ParamEnumArray.FromEnum(EAICombatType), category: "Common")]
	EAICombatType m_eAICombatType;

	[Attribute(defvalue: "1", uiwidget: UIWidgets.EditBox, desc: "Sets perception ability. Affects speed at which perception detects targets. Bigger value means proportionally faster detection.", params: "0 100 0.001", category: "Common")]
	float m_fPerceptionFactor;
	
	[Attribute(defvalue: "", desc: "Названия вейпоинтов", category: "Dynamic")]
    ref array<string> m_aWaypointsNames;
	
	protected ref array<SCR_AIWaypoint> m_Waypoints;
	protected IEntity m_Owner;
	
	protected bool CanSpawnGroup()
	{
		array<vector> aObserversPositions = {};
		array<int> playerIds = {};
		PlayerManager playerManager = GetGame().GetPlayerManager();
		IEntity player;
		SCR_DamageManagerComponent damageManager;
		playerManager.GetPlayers(playerIds);

		foreach (int playerId : playerIds)
		{
			player = playerManager.GetPlayerControlledEntity(playerId);
			if (!player)
				continue;

			damageManager = SCR_DamageManagerComponent.GetDamageManager(player);
			if (damageManager && damageManager.GetState() != EDamageState.DESTROYED)
				aObserversPositions.Insert(player.GetOrigin());
		}
		
		foreach (vector observerPos : aObserversPositions)
		{
			if (vector.Distance(observerPos, m_Owner.GetOrigin()) < m_Radius)
				return false;
		}
		

		return true;
	}
	
	protected override void EOnInit(IEntity owner)
	{
		IEntity main = owner.GetParent();
		SCR_ScenarioFrameworkLayerBase layer = SCR_ScenarioFrameworkLayerBase.Cast(main.FindComponent(SCR_ScenarioFrameworkLayerBase));
		IEntity trigger = layer.GetParentArea().GetOwner();
		PGT_TriggerCoreComponent core = PGT_TriggerCoreComponent.GetThis(trigger);
		core.RegisterGroup(owner, m_Dynamic);
		
		if (m_aWaypointsNames.Count() > 0)
		{
			foreach (string name : m_aWaypointsNames)
			{
				SCR_AIWaypoint waypoint = SCR_AIWaypoint.Cast(GetGame().GetWorld().FindEntityByName(name));
		      	if (waypoint) {
		            Print("PGT: Added waypoint - " + waypoint);
		            m_Waypoints.Insert(waypoint);
		        }  
			}
			
		}
	}
	
	protected override void OnPostInit(IEntity owner)
	{
		if (SCR_Global.IsEditMode())
			return;
	
		m_Waypoints = {};
		
		SetEventMask(owner, EntityEvent.INIT);
		
		if (m_Dynamic)
			SetEventMask(owner, EntityEvent.FRAME);
		
		m_Owner = owner;
	}
	
	SCR_AIGroup m_AIGroup;
	
	void SpawnGroup()
	{
		if (m_Dynamic && !CanSpawnGroup()) {
			Print("PGT: DYNAMIC Cant spawn group...");
			return;
		}
		
		vector mat[4] = {};
		m_Owner.GetTransform(mat);
		Resource resource = Resource.Load(m_GroupPrefab);
		
		Print("PGT: Prefab - " + resource.ToString());
		
		EntitySpawnParams params = new EntitySpawnParams();
		params.TransformMode = ETransformMode.OFFSET;
		params.Transform = mat;
	
		m_AIGroup = SCR_AIGroup.Cast(GetGame().SpawnEntityPrefab(resource, GetGame().GetWorld(), params));
		m_AIGroup.GetOnAgentAdded().Insert(CheckMemberCount);
	}

	protected void CheckMemberCount()
	{
		if (m_AIGroup.GetNumberOfMembersToSpawn() != m_AIGroup.GetAgentsCount())
			return;
		
		m_AIGroup.GetOnAgentAdded().Remove(CheckMemberCount);
		InitGroupComponents();
	}
	
	protected void InitGroupComponents()
	{
		array<AIAgent> agents = {};
		int count = m_AIGroup.GetAgents(agents);

		Print("PGT: Init group componentns...");
		
		if (count == 0)
			Print("PGT: Cant set AI Skills...");
		
		foreach (AIAgent agent : agents)
		{
			IEntity agentEntity = agent.GetControlledEntity();
			if (!agentEntity)
				continue;

			SCR_AICombatComponent combatComponent = SCR_AICombatComponent.Cast(agentEntity.FindComponent(SCR_AICombatComponent));
			if (combatComponent)
			{
				combatComponent.SetAISkill(m_eAISkill);
				combatComponent.SetCombatType(m_eAICombatType);
				combatComponent.SetPerceptionFactor(m_fPerceptionFactor);
			}
		}
		
		foreach (SCR_AIWaypoint waypoint : m_Waypoints)
		{
			m_AIGroup.AddWaypoint(waypoint);
		}
		
		m_AIGroup = null;
	}
	
	static PGT_GroupComponent GetThis(IEntity owner)
	{
	    PGT_GroupComponent component = PGT_GroupComponent.Cast(owner.FindComponent(PGT_GroupComponent));
	    if (component == null)
	    {
	        Print("PGT: PGT_GroupComponent not found.");
	        return null;
	    }
    
    	return component;
	}
}