[BaseContainerProps(), SCR_ContainerActionTitle()]
class PGT_ExecuteLastSkipAction : SCR_ScenarioFrameworkActionBase
{
	//------------------------------------------------------------------------------------------------
	override void OnActivate(IEntity object)
	{
		if (!CanActivate())
			return;

		Print("PGT: Activate : " + object.Type() + " --- " + object.GetName());
		SCR_ScenarioFrameworkLayerTask task = SCR_ScenarioFrameworkLayerTask.Cast(object.FindComponent(SCR_ScenarioFrameworkLayerTask));
		if (task) {
			
			Print("PGT: Activate layer");
			SCR_ScenarioFrameworkArea area = task.GetParentArea();
			PGT_TriggerCoreComponent core = PGT_TriggerCoreComponent.GetThis(area.GetOwner());
			if (core) {
				core.SetEndState(task);
				Print("PGT: Core avaliable");
			}
		}		
	}
}