#include <QItemSelectionModel>
#include "itoolmanager.h"

#include "selectiontool.h"

SelectionTool::SelectionTool(QItemSelectionModel* selectionModel,
                             QAbstractItemModel* model) :
    m_selectionModel(selectionModel),
    m_model(model)
{
    if (m_selectionModel)
        connect(m_selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                SLOT(selectionChanged(QItemSelection,QItemSelection)));
}

void SelectionTool::init(IToolManager *manager)
{
    m_manager = manager;
    addAction(Actions::EditSelectAll);
}

const char * SelectionTool::getActionSlot(Actions::Actions action) const
{
    if (action == Actions::EditSelectAll)
        return SLOT(onSelectAll());
    return 0;
}

QObject *SelectionTool::getReciever()
{
    return this;
}

bool SelectionTool::isActionEnabled(Actions::Actions action) const
{
    if (action == Actions::EditSelectAll)
        return m_selectionModel && m_model;
    return false;
}

void SelectionTool::onSelectAll()
{
    QItemSelection sel(m_model->index(0, 0), m_model->index(m_model->rowCount() - 1, 0));
    m_selectionModel->select(sel, QItemSelectionModel::Rows|QItemSelectionModel::ClearAndSelect);
}

void SelectionTool::onModelsChanged(QItemSelectionModel *selectionModel, QAbstractItemModel *model)
{
    m_selectionModel = selectionModel;
    m_model = model;
    m_manager->onActionChanged(Actions::EditSelectAll);

    connect(m_selectionModel, SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            SLOT(selectionChanged(QItemSelection,QItemSelection)));
    emit(selectionChanged(m_selectionModel));
}

void SelectionTool::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    (void)selected;
    (void)deselected;
    emit(selectionChanged(m_selectionModel));
}
