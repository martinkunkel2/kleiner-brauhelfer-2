#include "setmodeldatacommand.h"

SetModelDataCommand::SetModelDataCommand(QAbstractItemModel *model, int row, int col, const QVariant &value, QUndoCommand *parent) :
    QUndoCommand(parent),
    model(model),
    row(row),
    col(col),
    newValue(value)
{
    prevValue = model->index(row,col).data();
    QObject::connect(model, &QObject::destroyed, [this](){setObsolete(true);});
}

void SetModelDataCommand::undo()
{
    model->setData(model->index(row,col), prevValue);
}

void SetModelDataCommand::redo()
{
    model->setData(model->index(row,col), newValue);
}

int SetModelDataCommand::id() const
{
    return 1;
}

bool SetModelDataCommand::mergeWith(const QUndoCommand *other)
{
    if (other->id() != id())
        return false;
    const SetModelDataCommand* otherCmd = static_cast<const SetModelDataCommand*>(other);
    if (otherCmd->model != model || otherCmd->row != row || otherCmd->col != col)
        return false;
    newValue = otherCmd->newValue;
    return true;
}
