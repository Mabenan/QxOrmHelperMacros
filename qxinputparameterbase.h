#ifndef QXINPUTPARAMETERBASE_H
#define QXINPUTPARAMETERBASE_H

#include <QxOrmHelperMacros.Precompiled.h>

template<typename O, typename L>
class QxInputParameterBase : public qx::service::IxParameter
{
public:

    O instance;
    L list;
    qx_query query;
    QStringList columns;
    QStringList relations;
};
QX_REGISTER_CLASS_NAME_TEMPLATE_2(QxInputParameterBase);
#define REGISTER_INPUT_PARAMETER_BASE(type1, type2)\
namespace qx {\
template<> void register_class(QxClass<QxInputParameterBase<type1,type2>> &t) {\
    t.data(&QxInputParameterBase<type1,type2>::instance, QStringLiteral("instance"));\
    t.data(&QxInputParameterBase<type1,type2>::list, QStringLiteral("list"));\
    t.data(&QxInputParameterBase<type1,type2>::query, QStringLiteral("query"));\
    t.data(&QxInputParameterBase<type1,type2>::columns, QStringLiteral("columns"));\
    t.data(&QxInputParameterBase<type1,type2>::relations, QStringLiteral("relations"));\
}\
}
#endif // QXINPUTPARAMETERBASE_H
