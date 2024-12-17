[BaseContainerProps(), SCR_ContainerActionTitle()]
class PGT_ActionSpawnDynamicGroups : PGT_BaseReinformContainer
{
	
    protected override void Init(IEntity entity)
    {
        super.Init(entity); 
		Print("PGT: Action init yes");
		PGT_TriggerCoreComponent core = PGT_TriggerCoreComponent.GetThis(entity);
		core.SpawnDynamicGroups();
    }
}
