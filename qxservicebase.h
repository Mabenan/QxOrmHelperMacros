#ifndef QXSERVICEBASE_H
#define QXSERVICEBASE_H

#include <QxOrmHelperMacros.Precompiled.h>
template <class INPUT, class OUTPUT, class OBJECT, class LIST>
class QxServiceBase : public qx::service::QxService<INPUT, OUTPUT> {
  QX_REGISTER_FRIEND_CLASS(QxServiceBase)
public:
  QxServiceBase(const QString &sServiceName)
      : qx::service::QxService<INPUT, OUTPUT>(sServiceName) {
    ;
  }
  virtual ~QxServiceBase() { ; }
  typedef std::shared_ptr<INPUT> INPUT_ptr;
  typedef std::shared_ptr<OUTPUT> OUTPUT_ptr;
  using OBJECT_ptr = std::shared_ptr<OBJECT>;
  typedef std::shared_ptr<LIST> LIST_ptr;

public:
#ifdef _APP_CLIENT
  void count_() { qx::service::execute_client(this, QStringLiteral("count")); }

  void fetchById_() {
    qx::service::execute_client(this, QStringLiteral("fetchById"));
  }

  void fetchAll_() {
    qx::service::execute_client(this, QStringLiteral("fetchAll"));
  }

  void fetchByQuery_() {
    qx::service::execute_client(this, QStringLiteral("fetchByQuery"));
  }

  void insert_() {
    qx::service::execute_client(this, QStringLiteral("insert"));
  }
  void update_() {
    qx::service::execute_client(this, QStringLiteral("update"));
  }

  void save_() { qx::service::execute_client(this, QStringLiteral("save")); }

  void deleteById_() {
    qx::service::execute_client(this, QStringLiteral("deleteById"));
  }

  void deleteAll_() {
    qx::service::execute_client(this, QStringLiteral("deleteAll"));
  }

  void deleteByQuery_() {
    qx::service::execute_client(this, QStringLiteral("deleteByQuery"));
  }

  void destroyById_() {
    qx::service::execute_client(this, QStringLiteral("destroyById"));
  }

  void destroyAll_() {
    qx::service::execute_client(this, QStringLiteral("destroyAll"));
  }

  void destroyByQuery_() {
    qx::service::execute_client(this, QStringLiteral("destroyByQuery"));
  }

  void executeQuery_() {
    qx::service::execute_client(this, QStringLiteral("executeQuery"));
  }

  void exist_() { qx::service::execute_client(this, QStringLiteral("exist")); }

  void isValid_() {
    qx::service::execute_client(this, QStringLiteral("isValid"));
  }
  void callQuery_() {
    qx::service::execute_client(this, QStringLiteral("callQuery"));
  }

#else
  void count_() {
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    INPUT_ptr input = this->getInputParameter();
    output->error = qx::dao::count<OBJECT>(output->count, input->query);
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void fetchById_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'AuthUserService::fetchById()'");
      return;
    }
    typename qx::trait::get_primary_key<OBJECT>::type id = input->id;
    if (input->instance) {
      id = qx::trait::get_primary_key_value<OBJECT>::getPrimaryKeyValue(
          input->instance);
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    if (input->list) {
      output->list = input->list;
    } else {
      output->instance = OBJECT_ptr(new OBJECT());
      qx::trait::set_primary_key<OBJECT>::setPrimaryKey(output->instance, id);
    }
    if (output->instance && !qx::dao::exist(*output->instance)) {
      this->setMessageReturn(0, "ID doesn't exist in database : cannot fetch "
                                "item of type 'auth_user'");
      return;
    }
    if (output->list && (input->relations.count() == 0)) {
      output->error =
          qx::dao::fetch_by_id((*output->list), NULL, input->columns);
    } else if (output->list) {
      output->error = qx::dao::fetch_by_id_with_relation(input->relations,
                                                         (*output->list), NULL);
    } else if (input->relations.count() == 0) {
      output->error =
          qx::dao::fetch_by_id((*output->instance), NULL, input->columns);
    } else {
      output->error = qx::dao::fetch_by_id_with_relation(
          input->relations, (*output->instance), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void fetchAll_() {

    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'AuthUserService::fetchAll()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->list = LIST_ptr(new LIST());
    if (input->relations.count() == 0) {
      output->error = qx::dao::fetch_all((*output->list), NULL, input->columns);
    } else {
      output->error = qx::dao::fetch_all_with_relation(input->relations,
                                                       (*output->list), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void fetchByQuery_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::fetchByQuery()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->list = LIST_ptr(new LIST());
    if (input->relations.count() == 0) {
      output->error = qx::dao::fetch_by_query(input->query, (*output->list),
                                              NULL, input->columns);
    } else {
      output->error = qx::dao::fetch_by_query_with_relation(
          input->relations, input->query, (*output->list), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void insert_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::insert()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->instance = input->instance;
    output->list = input->list;
    if (!output->instance && !output->list) {
      this->setMessageReturn(
          0, "Nothing to insert in database (of type 'OBJECT')");
      return;
    }
    if (output->list && (input->relations.count() == 0)) {
      output->error = qx::dao::insert((*output->list), NULL);
    } else if (output->list) {
      output->error = qx::dao::insert_with_relation(input->relations,
                                                    (*output->list), NULL);
    } else if (input->relations.count() == 0) {
      output->error = qx::dao::insert((*output->instance), NULL);
    } else {
      output->error = qx::dao::insert_with_relation(input->relations,
                                                    (*output->instance), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void update_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::update()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->instance = input->instance;
    output->list = input->list;
    if (!output->instance && !output->list) {
      this->setMessageReturn(
          0, "Nothing to update in database (of type 'OBJECT')");
      return;
    }
    if (output->list && (input->relations.count() == 0)) {
      output->error = qx::dao::update_by_query(input->query, (*output->list),
                                               NULL, input->columns);
    } else if (output->list) {
      output->error = qx::dao::update_by_query_with_relation(
          input->relations, input->query, (*output->list), NULL);
    } else if (input->relations.count() == 0) {
      output->error = qx::dao::update_by_query(
          input->query, (*output->instance), NULL, input->columns);
    } else {
      output->error = qx::dao::update_by_query_with_relation(
          input->relations, input->query, (*output->instance), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void save_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(
          0,
          "invalid input parameter to call service 'OBJECT_services::save()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->instance = input->instance;
    output->list = input->list;
    if (!output->instance && !output->list) {
      this->setMessageReturn(0,
                             "Nothing to save in database (of type 'OBJECT')");
      return;
    }
    if (output->list && (input->relations.count() == 0)) {
      output->error = qx::dao::save((*output->list), NULL);
    } else if (output->list) {
      output->error =
          qx::dao::save_with_relation(input->relations, (*output->list), NULL);
    } else if (input->relations.count() == 0) {
      output->error = qx::dao::save((*output->instance), NULL);
    } else {
      output->error = qx::dao::save_with_relation(input->relations,
                                                  (*output->instance), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void deleteById_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::deleteById()'");
      return;
    }
    typename qx::trait::get_primary_key<OBJECT>::type id = input->id;
    if (input->instance) {
      id = qx::trait::get_primary_key_value<OBJECT>::getPrimaryKeyValue(input->instance);
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    if (input->list) {
      output->list = input->list;
    } else {
      output->instance = OBJECT_ptr(new OBJECT());
      qx::trait::set_primary_key<OBJECT>::setPrimaryKey(output->instance, id);
    }
    if (output->instance && !qx::dao::exist(*output->instance)) {
      this->setMessageReturn(
          0,
          "ID doesn't exist in database : cannot delete item of type 'OBJECT'");
      return;
    }
    if (output->list) {
      output->error = qx::dao::delete_by_id((*output->list), NULL);
    } else {
      output->error = qx::dao::delete_by_id((*output->instance), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void deleteAll_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::deleteAll()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->error = qx::dao::delete_all<OBJECT>(NULL);
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void deleteByQuery_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::deleteByQuery()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->error = qx::dao::delete_by_query<OBJECT>(input->query, NULL);
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void destroyById_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::destroyById()'");
      return;
    }
    typename qx::trait::get_primary_key<OBJECT>::type id = input->id;
    if (input->instance) {
      id = qx::trait::get_primary_key_value<OBJECT>::getPrimaryKeyValue(input->instance);
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    if (input->list) {
      output->list = input->list;
    } else {
      output->instance = OBJECT_ptr(new OBJECT());
      qx::trait::set_primary_key<OBJECT>::setPrimaryKey(output->instance, id);
    }
    if (output->instance && !qx::dao::exist(*output->instance)) {
      this->setMessageReturn(
          0, "ID doesn't exist in database : cannot destroy item "
             "of type 'OBJECT'");
      return;
    }
    if (output->list) {
      output->error = qx::dao::destroy_by_id((*output->list), NULL);
    } else {
      output->error = qx::dao::destroy_by_id((*output->instance), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void destroyAll_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::destroyAll()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->error = qx::dao::destroy_all<OBJECT>(NULL);
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void destroyByQuery_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::destroyByQuery()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->error = qx::dao::destroy_by_query<OBJECT>(input->query, NULL);
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void executeQuery_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::executeQuery()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->instance = input->instance;
    output->list = input->list;
    output->query = input->query;
    if (!output->instance && !output->list) {
      this->setMessageReturn(
          0, "No item of type 'OBJECT' defined to execute query");
      return;
    }
    if (output->list) {
      output->error =
          qx::dao::execute_query<LIST>(output->query, (*output->list), NULL);
    } else {
      output->error = qx::dao::execute_query<OBJECT>(output->query,
                                                     (*output->instance), NULL);
    }
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void exist_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(
          0,
          "invalid input parameter to call service 'OBJECT_services::exist()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->instance = input->instance;
    output->list = input->list;
    if (!output->instance && !output->list) {
      this->setMessageReturn(
          0, "No item of type 'OBJECT' defined to test if it exists or not");
      return;
    }
    if (output->list) {
      output->exist = qx::dao::exist((*output->list), NULL);
    } else {
      output->exist = qx::dao::exist((*output->instance), NULL);
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void isValid_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::isValid()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->instance = input->instance;
    output->list = input->list;
    if (!output->instance && !output->list) {
      this->setMessageReturn(0, "No item of type 'OBJECT' to validate");
      return;
    }
    if (output->list) {
      output->invalid = qx::validate(*output->list);
    } else {
      output->invalid = qx::validate(*output->instance);
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
  void callQuery_() {
    INPUT_ptr input = this->getInputParameter();
    if (!input) {
      this->setMessageReturn(0, "invalid input parameter to call service "
                                "'OBJECT_services::callQuery()'");
      return;
    }
    OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
    output->query = input->query;
    output->error = qx::dao::call_query(output->query, NULL);
    if (output->error.isValid()) {
      this->setMessageReturn(0, output->error.text());
      return;
    }
    this->setOutputParameter(output);
    this->setMessageReturn(true);
  }
#endif
public:
#ifdef _APP_CLIENT
  OBJECT_ptr fetchById(typename qx::trait::get_primary_key<OBJECT>::type id,
                       const QStringList &columns = QStringList(),
                       const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->id = id;
    input->columns = columns;
    input->relations = relations;
    this->setInputParameter(input);
    this->fetchById_();
    if (!this->isValidWithOutput()) {
      return OBJECT_ptr();
    }
    OUTPUT_ptr output = this->getOutputParameter();
    return output->instance;
  }

  QSqlError fetchById(OBJECT_ptr &p, const QStringList &columns = QStringList(),
                      const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    input->columns = columns;
    input->relations = relations;
    this->setInputParameter(input);
    this->fetchById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    p = output->instance;
    return error;
  }
  QSqlError fetchById(LIST_ptr &lst, const QStringList &columns = QStringList(),
                      const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    input->columns = columns;
    input->relations = relations;
    this->setInputParameter(input);
    this->fetchById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }
  QSqlError fetchAll(LIST_ptr &lst, const QStringList &columns = QStringList(),
                     const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    input->columns = columns;
    input->relations = relations;
    this->setInputParameter(input);
    this->fetchAll_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }
  QSqlError fetchByQuery(const qx_query &query, LIST_ptr &lst,
                         const QStringList &columns = QStringList(),
                         const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->query = query;
    input->list = lst;
    input->columns = columns;
    input->relations = relations;
    this->setInputParameter(input);
    this->fetchByQuery_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }

  QSqlError insert(OBJECT_ptr &p,
                   const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    input->relations = relations;
    this->setInputParameter(input);
    this->insert_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    p = output->instance;
    return error;
  }
  QSqlError insert(LIST_ptr &lst,
                   const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    input->relations = relations;
    this->setInputParameter(input);
    this->insert_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }
  QSqlError update(OBJECT_ptr &p, const qx_query &query = qx_query(),
                   const QStringList &columns = QStringList(),
                   const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    input->query = query;
    input->columns = columns;
    input->relations = relations;
    this->setInputParameter(input);
    this->update_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    p = output->instance;
    return error;
  }
  QSqlError update(LIST_ptr &lst, const qx_query &query = qx_query(),
                   const QStringList &columns = QStringList(),
                   const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    input->query = query;
    input->columns = columns;
    input->relations = relations;
    this->setInputParameter(input);
    this->update_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }
  QSqlError save(OBJECT_ptr &p, const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    input->relations = relations;
    this->setInputParameter(input);
    this->save_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    p = output->instance;
    return error;
  }
  QSqlError save(LIST_ptr &lst, const QStringList &relations = QStringList()) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    input->relations = relations;
    this->setInputParameter(input);
    this->save_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }
  QSqlError deleteById(QString name) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->name = name;
    this->setInputParameter(input);
    this->deleteById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    return error;
  }
  QSqlError deleteById(OBJECT_ptr &p) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    this->setInputParameter(input);
    this->deleteById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    p = output->instance;
    return error;
  }
  QSqlError deleteById(LIST_ptr &lst) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    this->setInputParameter(input);
    this->deleteById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }
  QSqlError deleteAll() {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    this->setInputParameter(input);
    this->deleteAll_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    return error;
  }
  QSqlError deleteByQuery(const qx_query &query) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->query = query;
    this->setInputParameter(input);
    this->deleteByQuery_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    return error;
  }
  QSqlError destroyById(QString name) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->name = name;
    this->setInputParameter(input);
    this->destroyById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    return error;
  }
  QSqlError destroyById(OBJECT_ptr &p) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    this->setInputParameter(input);
    this->destroyById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    p = output->instance;
    return error;
  }
  QSqlError destroyById(LIST_ptr &lst) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    this->setInputParameter(input);
    this->destroyById_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lst = output->list;
    return error;
  }
  QSqlError destroyAll() {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    this->setInputParameter(input);
    this->destroyAll_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    return error;
  }
  QSqlError destroyByQuery(const qx_query &query) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->query = query;
    this->setInputParameter(input);
    this->destroyByQuery_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    return error;
  }

  QSqlError executeQuery(qx_query &query, OBJECT_ptr &p) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->query = query;
    input->instance = p;
    this->setInputParameter(input);
    this->executeQuery_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    query = output->query;
    p = output->instance;
    return error;
  }
  QSqlError executeQuery(qx_query &query, LIST_ptr &lst) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->query = query;
    input->list = lst;
    this->setInputParameter(input);
    this->executeQuery_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    query = output->query;
    lst = output->list;
    return error;
  }
  QSqlError callQuery(qx_query &query) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->query = query;
    this->setInputParameter(input);
    this->callQuery_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    query = output->query;
    return error;
  }
  qx_bool exist(OBJECT_ptr &p) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    this->setInputParameter(input);
    this->exist_();
    if (!this->isValidWithOutput()) {
      return this->m_bMessageReturn;
    }
    OUTPUT_ptr output = this->getOutputParameter();
    return output->exist;
  }
  qx_bool exist(LIST_ptr &lst) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    this->setInputParameter(input);
    this->exist_();
    if (!this->isValidWithOutput()) {
      return this->m_bMessageReturn;
    }
    OUTPUT_ptr output = this->getOutputParameter();
    return output->exist;
  }
  QSqlError count(long &lCount, const qx_query &query = qx_query()) {
    lCount = 0;
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->query = query;
    this->setInputParameter(input);
    this->count_();
    if (!this->isValidWithOutput()) {
      return QSqlError("", this->m_bMessageReturn.getDesc(),
                       QSqlError::UnknownError);
    }
    OUTPUT_ptr output = this->getOutputParameter();
    QSqlError error = output->error;
    lCount = output->count;
    return error;
  }
  qx::QxInvalidValueX isValid(OBJECT_ptr &p) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->instance = p;
    this->setInputParameter(input);
    this->isValid_();
    if (!this->isValidWithOutput()) {
      qx::QxInvalidValueX invalid;
      invalid.insert(this->m_bMessageReturn.getDesc());
      return invalid;
    }
    OUTPUT_ptr output = this->getOutputParameter();
    return output->invalid;
  }
  qx::QxInvalidValueX isValid(LIST_ptr &lst) {
    this->m_pOutputParameter.reset();
    INPUT_ptr input = INPUT_ptr(new INPUT());
    input->list = lst;
    this->setInputParameter(input);
    this->isValid_();
    if (!this->isValidWithOutput()) {
      qx::QxInvalidValueX invalid;
      invalid.insert(this->m_bMessageReturn.getDesc());
      return invalid;
    }
    OUTPUT_ptr output = this->getOutputParameter();
    return output->invalid;
  }
#endif
};
QX_REGISTER_CLASS_NAME_TEMPLATE_4(QxServiceBase);
#define REGISTER_SERVICE_BASE(class)                                           \
  namespace qx {                                                               \
  template <> void register_class(QxClass<class> &t) {                         \
                                                                               \
    t.fct_0<void>(std::mem_fn(&class ::count_), QStringLiteral("count"));      \
    t.fct_0<void>(std::mem_fn(&class ::fetchById_),                            \
                  QStringLiteral("fetchById"));                                \
    t.fct_0<void>(std::mem_fn(&class ::fetchAll_),                             \
                  QStringLiteral("fetchAll"));                                 \
    t.fct_0<void>(std::mem_fn(&class ::fetchByQuery_),                         \
                  QStringLiteral("fetchByQuery"));                             \
    t.fct_0<void>(std::mem_fn(&class ::insert_), QStringLiteral("insert"));    \
    t.fct_0<void>(std::mem_fn(&class ::update_), QStringLiteral("update"));    \
    t.fct_0<void>(std::mem_fn(&class ::save_), QStringLiteral("save"));        \
    t.fct_0<void>(std::mem_fn(&class ::deleteById_),                           \
                  QStringLiteral("deleteById"));                               \
    t.fct_0<void>(std::mem_fn(&class ::deleteAll_),                            \
                  QStringLiteral("deleteAll"));                                \
    t.fct_0<void>(std::mem_fn(&class ::deleteByQuery_),                        \
                  QStringLiteral("deleteByQuery"));                            \
    t.fct_0<void>(std::mem_fn(&class ::destroyById_),                          \
                  QStringLiteral("destroyById"));                              \
    t.fct_0<void>(std::mem_fn(&class ::destroyAll_),                           \
                  QStringLiteral("destroyAll"));                               \
    t.fct_0<void>(std::mem_fn(&class ::destroyByQuery_),                       \
                  QStringLiteral("destroyByQuery"));                           \
    t.fct_0<void>(std::mem_fn(&class ::executeQuery_),                         \
                  QStringLiteral("executeQuery"));                             \
    t.fct_0<void>(std::mem_fn(&class ::exist_), QStringLiteral("exist"));      \
    t.fct_0<void>(std::mem_fn(&class ::isValid_), QStringLiteral("isValid"));  \
    t.fct_0<void>(std::mem_fn(&class ::callQuery_),                            \
                  QStringLiteral("callQuery"));                                \
  }                                                                            \
  }
#endif // QXSERVICEBASE_H
