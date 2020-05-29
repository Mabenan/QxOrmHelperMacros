#ifndef QXSERVICEBASE_H
#define QXSERVICEBASE_H

#include <QxOrmHelperMacros.Precompiled.h>
template <class INPUT, class OUTPUT, class OBJECT, class LIST>
class QxServiceBase : public qx::service::QxService<INPUT, OUTPUT> {
  QX_REGISTER_FRIEND_CLASS(QxServiceBase)
public:
      QxServiceBase(const QString & sServiceName) : qx::service::QxService<INPUT, OUTPUT>(sServiceName) { ; }
      virtual ~QxServiceBase() { ; }
  typedef std::shared_ptr<INPUT> INPUT_ptr;
  typedef std::shared_ptr<OUTPUT> OUTPUT_ptr;
  using OBJECT_ptr = std::shared_ptr<OBJECT>;
  typedef std::shared_ptr<LIST> LIST_ptr;
public:
#ifdef _APP_CLIENT
  void count_() {
    qx::service::execute_client(this, QStringLiteral("count"));
  }

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

  void save_() {
    qx::service::execute_client(this, QStringLiteral("save"));
  }

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

  void exist_() {
    qx::service::execute_client(this, QStringLiteral("exist"));
  }

  void isValid_() {
    qx::service::execute_client(this, QStringLiteral("isValid"));
  }
  void callQuery_() {
    qx::service::execute_client(this, QStringLiteral("callQuery"));
  }

#else
  void count_(){
      OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
      INPUT_ptr input = this->getInputParameter();
      output->error = qx::dao::count<OBJECT>(output->count, input->query);
      this->setOutputParameter(output);
      this->setMessageReturn(true);
    }
  void fetchById_() {
      INPUT_ptr input = this->getInputParameter();
      if (!input) {
        this->setMessageReturn(
            0,
            "invalid input parameter to call service 'AuthUserService::fetchById()'");
        return;
      }
      QString name = input->name;
      if (input->instance) {
        name = input->instance->name;
      }
      OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
      if (input->list) {
        output->list = input->list;
      } else {
        output->instance = OBJECT_ptr(new OBJECT());
        output->instance->name = name;
      }
      if (output->instance && !qx::dao::exist(*output->instance)) {
        this->setMessageReturn(
            0, "ID doesn't exist in database : cannot fetch item of type 'auth_user'");
        return;
      }
      if (output->list && (input->relations.count() == 0)) {
        output->error = qx::dao::fetch_by_id((*output->list), NULL, input->columns);
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
      if (! input) { this->setMessageReturn(0, "invalid input parameter to call service 'AuthUserService::fetchAll()'"); return; }
      OUTPUT_ptr output = OUTPUT_ptr(new OUTPUT());
      output->list = LIST_ptr(new LIST());
      if (input->relations.count() == 0) { output->error = qx::dao::fetch_all((* output->list), NULL, input->columns); }
      else { output->error = qx::dao::fetch_all_with_relation(input->relations, (* output->list), NULL); }
      if (output->error.isValid()) { this->setMessageReturn(0, output->error.text()); return; }
      this->setOutputParameter(output);
      this->setMessageReturn(true);
  }
  void fetchByQuery_(){}
  void insert_(){}
  void update_(){}
  void save_(){}
  void deleteById_(){}
  void deleteAll_(){}
  void deleteByQuery_(){}
  void destroyById_(){}
  void destroyAll_(){}
  void destroyByQuery_(){}
  void executeQuery_(){}
  void exist_(){}
  void isValid_(){}
  void callQuery_(){}
#endif
 public:

#ifdef _APP_CLIENT
  OBJECT_ptr fetchById(QString name,
                          const QStringList &columns = QStringList(),
                          const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->name = name;
      input->columns = columns;
      input->relations = relations;
      this->setInputParameter(input);
      this->fetchById_();
      if (! this->isValidWithOutput()) { return OBJECT_ptr(); }
      OUTPUT_ptr output = this->getOutputParameter();
      return output->instance;
   }

  QSqlError fetchById(OBJECT_ptr &p,
                      const QStringList &columns = QStringList(),
                      const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      input->columns = columns;
      input->relations = relations;
      this->setInputParameter(input);
      this->fetchById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      p = output->instance;
      return error;
   }
  QSqlError fetchById(LIST_ptr &lst,
                      const QStringList &columns = QStringList(),
                      const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      input->columns = columns;
      input->relations = relations;
      this->setInputParameter(input);
      this->fetchById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }
  QSqlError fetchAll(LIST_ptr &lst,
                     const QStringList &columns = QStringList(),
                     const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      input->columns = columns;
      input->relations = relations;
      this->setInputParameter(input);
      this->fetchAll_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }
  QSqlError fetchByQuery(const qx_query &query, LIST_ptr &lst,
                         const QStringList &columns = QStringList(),
                         const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->query = query;
      input->list = lst;
      input->columns = columns;
      input->relations = relations;
      this->setInputParameter(input);
      this->fetchByQuery_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }

  QSqlError insert(OBJECT_ptr &p,
                   const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      input->relations = relations;
      this->setInputParameter(input);
      this->insert_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      p = output->instance;
      return error;
   }
  QSqlError insert(LIST_ptr &lst,
                   const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      input->relations = relations;
      this->setInputParameter(input);
      this->insert_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }
  QSqlError update(OBJECT_ptr &p, const qx_query &query = qx_query(),
                   const QStringList &columns = QStringList(),
                   const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      input->query = query;
      input->columns = columns;
      input->relations = relations;
      this->setInputParameter(input);
      this->update_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      p = output->instance;
      return error;
   }
  QSqlError update(LIST_ptr &lst, const qx_query &query = qx_query(),
                   const QStringList &columns = QStringList(),
                   const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      input->query = query;
      input->columns = columns;
      input->relations = relations;
      this->setInputParameter(input);
      this->update_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }
  QSqlError save(OBJECT_ptr &p,
                 const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      input->relations = relations;
      this->setInputParameter(input);
      this->save_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      p = output->instance;
      return error;
   }
  QSqlError save(LIST_ptr &lst,
                 const QStringList &relations = QStringList()){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      input->relations = relations;
      this->setInputParameter(input);
      this->save_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }
  QSqlError deleteById(QString name){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->name = name;
      this->setInputParameter(input);
      this->deleteById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      return error;
   }
  QSqlError deleteById(OBJECT_ptr &p){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      this->setInputParameter(input);
      this->deleteById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      p = output->instance;
      return error;
   }
  QSqlError deleteById(LIST_ptr &lst){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      this->setInputParameter(input);
      this->deleteById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }
  QSqlError deleteAll(){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      this->setInputParameter(input);
      this->deleteAll_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      return error;
   }
  QSqlError deleteByQuery(const qx_query &query){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->query = query;
      this->setInputParameter(input);
      this->deleteByQuery_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      return error;
   }
  QSqlError destroyById(QString name){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->name = name;
      this->setInputParameter(input);
      this->destroyById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      return error;
   }
  QSqlError destroyById(OBJECT_ptr &p){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      this->setInputParameter(input);
      this->destroyById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      p = output->instance;
      return error;
   }
  QSqlError destroyById(LIST_ptr &lst){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      this->setInputParameter(input);
      this->destroyById_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lst = output->list;
      return error;
   }
  QSqlError destroyAll(){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      this->setInputParameter(input);
      this->destroyAll_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      return error;
   }
  QSqlError destroyByQuery(const qx_query &query){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->query = query;
      this->setInputParameter(input);
      this->destroyByQuery_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      return error;
   }

  QSqlError executeQuery(qx_query &query, OBJECT_ptr &p){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->query = query;
      input->instance = p;
      this->setInputParameter(input);
      this->executeQuery_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      query = output->query;
      p = output->instance;
      return error;
   }
  QSqlError executeQuery(qx_query &query, LIST_ptr &lst){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->query = query;
      input->list = lst;
      this->setInputParameter(input);
      this->executeQuery_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      query = output->query;
      lst = output->list;
      return error;
   }
  QSqlError callQuery(qx_query &query){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->query = query;
      this->setInputParameter(input);
      this->callQuery_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      query = output->query;
      return error;
   }
  qx_bool exist(OBJECT_ptr &p){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      this->setInputParameter(input);
      this->exist_();
      if (! this->isValidWithOutput()) { return this->m_bMessageReturn; }
      OUTPUT_ptr output = this->getOutputParameter();
      return output->exist;
   }
  qx_bool exist(LIST_ptr &lst){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      this->setInputParameter(input);
      this->exist_();
      if (! this->isValidWithOutput()) { return this->m_bMessageReturn; }
      OUTPUT_ptr output = this->getOutputParameter();
      return output->exist;
   }
  QSqlError count(long &lCount, const qx_query &query = qx_query()){
      lCount = 0;
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->query = query;
      this->setInputParameter(input);
      this->count_();
      if (! this->isValidWithOutput()) { return QSqlError("", this->m_bMessageReturn.getDesc(), QSqlError::UnknownError); }
      OUTPUT_ptr output = this->getOutputParameter();
      QSqlError error = output->error;
      lCount = output->count;
      return error;
   }
  qx::QxInvalidValueX isValid(OBJECT_ptr &p){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->instance = p;
      this->setInputParameter(input);
      this->isValid_();
      if (! this->isValidWithOutput()) { qx::QxInvalidValueX invalid; invalid.insert(this->m_bMessageReturn.getDesc()); return invalid; }
      OUTPUT_ptr output = this->getOutputParameter();
      return output->invalid;
   }
  qx::QxInvalidValueX isValid(LIST_ptr &lst){
      this->m_pOutputParameter.reset();
      INPUT_ptr input = INPUT_ptr(new INPUT());
      input->list = lst;
      this->setInputParameter(input);
      this->isValid_();
      if (! this->isValidWithOutput()) { qx::QxInvalidValueX invalid; invalid.insert(this->m_bMessageReturn.getDesc()); return invalid; }
      OUTPUT_ptr output = this->getOutputParameter();
      return output->invalid;
   }
#endif
};
  QX_REGISTER_CLASS_NAME_TEMPLATE_4(QxServiceBase);
  #define REGISTER_SERVICE_BASE(class)\
  namespace qx {\
  template<> void register_class(QxClass<class> &t) {\
    \
  t.fct_0<void>(std::mem_fn(&class::count_), QStringLiteral("count"));\
  t.fct_0<void>(std::mem_fn(&class::fetchById_),\
                QStringLiteral("fetchById"));\
  t.fct_0<void>(std::mem_fn(&class::fetchAll_),\
                QStringLiteral("fetchAll"));\
  t.fct_0<void>(std::mem_fn(&class::fetchByQuery_),\
                QStringLiteral("fetchByQuery"));\
  t.fct_0<void>(std::mem_fn(&class::insert_),\
                QStringLiteral("insert"));\
  t.fct_0<void>(std::mem_fn(&class::update_),\
                QStringLiteral("update"));\
  t.fct_0<void>(std::mem_fn(&class::save_), QStringLiteral("save"));\
  t.fct_0<void>(std::mem_fn(&class::deleteById_),\
                QStringLiteral("deleteById"));\
  t.fct_0<void>(std::mem_fn(&class::deleteAll_),\
                QStringLiteral("deleteAll"));\
  t.fct_0<void>(std::mem_fn(&class::deleteByQuery_),\
                QStringLiteral("deleteByQuery"));\
  t.fct_0<void>(std::mem_fn(&class::destroyById_),\
                QStringLiteral("destroyById"));\
  t.fct_0<void>(std::mem_fn(&class::destroyAll_),\
                QStringLiteral("destroyAll"));\
  t.fct_0<void>(std::mem_fn(&class::destroyByQuery_),\
                QStringLiteral("destroyByQuery"));\
  t.fct_0<void>(std::mem_fn(&class::executeQuery_),\
                QStringLiteral("executeQuery"));\
  t.fct_0<void>(std::mem_fn(&class::exist_), QStringLiteral("exist"));\
  t.fct_0<void>(std::mem_fn(&class::isValid_),\
                QStringLiteral("isValid"));\
  t.fct_0<void>(std::mem_fn(&class::callQuery_),\
                QStringLiteral("callQuery"));\
  }\
  }
#endif // QXSERVICEBASE_H
