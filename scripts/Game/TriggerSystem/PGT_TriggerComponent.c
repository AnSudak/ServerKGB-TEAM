[ComponentEditorProps(category: "PGT/Triggers/init", description: "Trigger Component")]
class PGT_TriggerComponentClass : ScriptComponentClass
{
}

class PGT_TriggerComponent : ScriptComponent
{
	
	[Attribute(defvalue: "1", desc: "Регестировать ли этот триггер?", category: "Main")]
	protected bool m_Registered;
	
	[Attribute(defvalue: "Мавес", desc: "Название триггера", category: "Main")]
	protected string m_Name;
	
	[Attribute(defvalue: "LayerName", desc: "Название слоя", category: "Main")]
	protected string m_LayerName;
	
	[Attribute(defvalue: "1", desc: "Активация одной рандомной", category: "Main")]
	protected bool m_TaskRandom;
	
	[Attribute(defvalue: "TaskList", desc: "Список задач", category: "Main")]
	ref array<string> m_TaskList;
	
	protected override void EOnInit(IEntity owner)
	{
		if (m_Registered) {
			PGT_TriggerManagerComponent manager = PGT_TriggerManagerComponent.GetThis();
			manager.RegisterTrigger(owner);
		} else {
			Print("PGT: Not Registered trigger - " + owner.GetName());
		}
	}
	
	
	//------------------------------------------------------------------------------------------------
	protected override void OnPostInit(IEntity owner)
	{
		if (SCR_Global.IsEditMode())
			return;
		
		SetEventMask(owner, EntityEvent.INIT);
	}
	
	void ActivateTask()
	{
		if (m_TaskList.IsEmpty() || !m_TaskList)
			return Print("PGT: Task list is empty...");
			
		if (m_TaskRandom) 
			PGT_TriggerHelper.ActivateLayer(m_TaskList.GetRandomElement());
		else 
		{
			foreach (string name : m_TaskList)
			{
				PGT_TriggerHelper.ActivateLayer(name);
			};
		};
		
		Print("PGT: Sending to web");
		PGT_TriggerManagerComponent TriggerManager = PGT_TriggerManagerComponent.GetThis();
		
		string missionName = GetGame().GetMissionName();
		if (missionName.IsEmpty())
			missionName = "unknown";
		
		
//		PG_JsonTrigger json = new PG_JsonTrigger();
//		json.Compile(missionName, m_Name, TriggerManager.restartCount, System.GetUnixTime(), GetGame().GetPlayerManager().GetPlayerCount().ToString());
//		PG_Request.SendPostNow(json.AsString(), "postHandler/");
	}
	
	static PGT_TriggerComponent GetThis(IEntity owner)
	{
	    PGT_TriggerComponent component = PGT_TriggerComponent.Cast(owner.FindComponent(PGT_TriggerComponent));
	    if (component == null)
	    {
	        Print("PGT: PGT_TriggerComponent not found.");
	        return null;
	    }
    
    	return component;
	}
}