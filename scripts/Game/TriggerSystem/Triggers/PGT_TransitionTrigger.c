[BaseContainerProps(), SCR_ContainerActionTitle()]
class PGT_TransitionTrigger : SCR_ScenarioFrameworkActionBase
{
	//------------------------------------------------------------------------------------------------
	override void OnActivate(IEntity object)
	{
		if (!CanActivate())
			return;

		Print("PGT: Transition Trigger");
	
		PGT_TriggerManagerComponent manager = PGT_TriggerManagerComponent.GetThis();
		manager.StartRandomTrigger();
		
	}
}