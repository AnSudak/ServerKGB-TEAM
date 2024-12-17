[ComponentEditorProps(category: "PGT/", description: "Trigger Component")]
class PGT_TriggerManagerComponentClass : ScriptComponentClass
{
}

class PG_Randoms
{
	protected ref RandomGenerator random;
	void PG_Randoms()
	{
		random = new RandomGenerator();
	}
	
	int GenerateSeed()
		return System.GetFPS() * System.GetTickCount() + System.GetUnixTime();
	
	int RandomInt(int min, int max)
	{
		random.SetSeed(GenerateSeed());
		return random.RandInt(min, max);
	}
}

class PGT_TriggerManagerComponent : ScriptComponent
{
	[Attribute(defvalue: "1", desc: "Перезапустить сервер если список пустой", category: "Main")]
	protected bool m_RestartIfListIsEmpty;
	
	[Attribute(defvalue: "", desc: "Список засад", category: "Main")]
	ref array<string> m_AmbushList;
	
	protected ref PG_Randoms random;
	protected ref array<IEntity> listOfTriggers;
	protected string currentAmbush;
	int restartCount;
	
	void RegisterTrigger(IEntity entity)
	{
		Print("PGT: New Trigger Registered: " + entity.GetName());
		listOfTriggers.Insert(entity)
	}
	
	protected override void EOnInit(IEntity owner)
	{
		if (!random) 
			random = new PG_Randoms();
		
		GetGame().GetCallqueue().CallLater(StartRandomTrigger, 10000, false);
	}
	
	void StartRandomTrigger()
	{		
		if (restartCount > 2) {
			DelayedRestart();
			return;
		}
		
		if (!m_AmbushList.IsEmpty())
		{
			if (currentAmbush != "")
				PGT_TriggerHelper.RestoreLayer(currentAmbush);
			
			string ambush = m_AmbushList.GetRandomElement();
			Print("PGT: Spawning ambush.. " + ambush);
			currentAmbush = ambush;
			PGT_TriggerHelper.ActivateLayer(ambush);
		};
		
		if (!listOfTriggers.IsEmpty()) {
			IEntity trigger;
			//Пиздец блять как же рандом криво работает в ArmaR
			for (int i = 0; i < random.RandomInt(5, 50); i++) 
			{
				trigger = listOfTriggers.GetRandomElement();
			}
			
			listOfTriggers.RemoveItem(trigger);
			
			PGT_TriggerCoreComponent core = PGT_TriggerCoreComponent.GetThis(trigger);
			core.StartTrigger(trigger);
			restartCount++;
		} else if (m_RestartIfListIsEmpty) {	
			Print("PGT: Trigger list is empty...");
			DelayedRestart();
		};
	}
	
	
	void DelayedRestart()
	{
		GetGame().GetCallqueue().CallLater(Alert, 1000, false,  "Рестарт сервера через 10 секунд");
		GetGame().GetCallqueue().CallLater(RestartMission, 10000, false);
	}
	
	void Alert(string message)
	{
		Rpc(RpcAsk_ShowMessage, message);
	}
	
	void RestartMission()
	{
		SCR_GameModeEndData endData = SCR_GameModeEndData.CreateSimple(EGameOverTypes.SERVER_RESTART, 0,0);
		SCR_BaseGameMode.Cast(GetGame().GetGameMode()).EndGameMode(endData);
	}
	
	//------------------------------------------------------------------------------------------------
	protected override void OnPostInit(IEntity owner)
	{
		if (SCR_Global.IsEditMode())
			return;
		
		listOfTriggers = {};
		currentAmbush = "";
		restartCount = 0;
		
		SetEventMask(owner, EntityEvent.INIT);
	}
		
	[RplRpc(RplChannel.Reliable, RplRcver.Broadcast)]
	void RpcAsk_ShowMessage(string content)
	{
		PlayerController controller = GetGame().GetPlayerController();
		if (!controller)
			return;
		
		SCR_ChatComponent chatComponent = SCR_ChatComponent.Cast(controller.FindComponent(SCR_ChatComponent));
		if (!chatComponent)
			return;

		chatComponent.ShowMessage(content);
	};
	
	static PGT_TriggerManagerComponent GetThis()
	{
        SCR_BaseGameMode gamemode = SCR_BaseGameMode.Cast(GetGame().GetGameMode());
	    PGT_TriggerManagerComponent manager = PGT_TriggerManagerComponent.Cast(gamemode.FindComponent(PGT_TriggerManagerComponent));
	    
	    if (manager == null)
	    {
	        Print("PGT: PGT_TriggerManagerComponent not found in the game mode.");
	        return null;
	    }
    
    	return manager;
	}
}