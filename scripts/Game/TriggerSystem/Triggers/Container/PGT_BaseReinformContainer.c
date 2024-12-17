[BaseContainerProps(), SCR_ContainerActionTitle()]
class PGT_BaseReinformContainer
{
	
	[Attribute(defvalue: "0", desc: "Игнорировать количество AI", category: "Main")]
	protected bool m_IgnoreAICount;
	
	[Attribute(defvalue: "0", desc: "Индекс задачи", category: "Main")]
	protected int m_Index;
	
	[Attribute(defvalue: "10", desc: "Количество противника для запуска", category: "Main")]
	protected int m_CountAI;
	
	[Attribute(defvalue: "0", desc: "Сколько нужно подождать тиков что бы запустить следующее действие", category: "Main")]
	protected int m_tickDelayForStart;
	
	IEntity	m_Entity;
	
	bool GetIgnoreAI()
		return m_IgnoreAICount;
	
	int GetCountAI()
		return m_CountAI;
	
	int GetIndex()
		return m_Index;
	
	int GetTicks()
		return m_tickDelayForStart;
	
	//------------------------------------------------------------------------------------------------
	//!
	//! \param[in] entity
	void Init(IEntity entity)
	{
		m_Entity = entity;
	}
}