#ifndef QXOUTPUTPARAMATERBASE_H
#define QXOUTPUTPARAMATERBASE_H

#include <QxOrmHelperMacros.Precompiled.h>
template<typename O, typename L>
class QxOutputParamaterBase : public qx::service::IxParameter
{
public:

    O instance;
    L list;
    QSqlError error;
    qx::QxInvalidValueX invalid;
    qx_query query;
    long count;
    qx_bool exist;
};
QX_REGISTER_CLASS_NAME_TEMPLATE_2(QxOutputParamaterBase);
#define REGISTER_OUTPUT_PARAMETER_BASE(type1, type2)\
namespace qx {\
template<> void register_class(QxClass<QxOutputParamaterBase<type1,type2>> &t) {\
    t.data(&QxOutputParamaterBase<type1,type2>::instance, QStringLiteral("instance"));\
    t.data(&QxOutputParamaterBase<type1,type2>::list, QStringLiteral("list"));\
    t.data(&QxOutputParamaterBase<type1,type2>::error, QStringLiteral("error"));\
    t.data(&QxOutputParamaterBase<type1,type2>::invalid, QStringLiteral("invalid"));\
    t.data(&QxOutputParamaterBase<type1,type2>::query, QStringLiteral("query"));\
    t.data(&QxOutputParamaterBase<type1,type2>::count, QStringLiteral("count"));\
    t.data(&QxOutputParamaterBase<type1,type2>::exist, QStringLiteral("exist"));\
}\
}
#endif // QXOUTPUTPARAMATERBASE_H
