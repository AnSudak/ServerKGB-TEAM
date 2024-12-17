class PGT_TriggerHelper
{
	static void ActivateLayer(string taskName)
	{
		IEntity object;
		SCR_ScenarioFrameworkLayerBase layer;	
		object = GetGame().GetWorld().FindEntityByName(taskName);
		
		if (!object)
		{
			Print(string.Format("PGT: Can't spawn object set in slot %1. Slot doesn't exist", taskName), LogLevel.ERROR);
			return;
		}

		layer = SCR_ScenarioFrameworkLayerBase.Cast(object.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (!layer)
		{
			Print(string.Format("PGT: Can't spawn object %1 - the slot doesn't have SCR_ScenarioFrameworkLayerBase component", taskName), LogLevel.ERROR);
			return;
		}
		
		if (layer.GetActivationType() != SCR_ScenarioFrameworkEActivationType.ON_TRIGGER_ACTIVATION)
		{
			Print(string.Format("PGT: Can't spawn object %1 - the slot has %2 activation type set instead of %3", taskName, layer.GetActivationType(), SCR_ScenarioFrameworkEActivationType.ON_TRIGGER_ACTIVATION), LogLevel.ERROR);
			return;
		}

		layer.Init(null, SCR_ScenarioFrameworkEActivationType.ON_TRIGGER_ACTIVATION);
		layer.SetActivationType(SCR_ScenarioFrameworkEActivationType.ON_TRIGGER_ACTIVATION);
	}
	
	static void RestoreLayer(string taskName)
	{
		IEntity object;
		SCR_ScenarioFrameworkLayerBase layer;	
		object = GetGame().GetWorld().FindEntityByName(taskName);
		
		if (!object)
		{
			Print(string.Format("PGT: Can't spawn object set in slot %1. Slot doesn't exist", taskName), LogLevel.ERROR);
			return;
		}

		layer = SCR_ScenarioFrameworkLayerBase.Cast(object.FindComponent(SCR_ScenarioFrameworkLayerBase));
		if (!layer)
		{
			Print(string.Format("PGT: Can't spawn object %1 - the slot doesn't have SCR_ScenarioFrameworkLayerBase component", taskName), LogLevel.ERROR);
			return;
		}

		layer.RestoreToDefault(true, true);
	}
}