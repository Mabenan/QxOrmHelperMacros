#ifndef QXMODELBASE_H
#define QXMODELBASE_H

#include <QxModelView.h>
#include <QxModelView/IxModel.h>
#include <QxOrm.h>

template <typename KEY_TYPE> class QxModelBase : public qx::IxModel {
public:
  QxModelBase(QObject *parent = nullptr) : qx::IxModel(parent) {}

public:
  QHash<int, QByteArray> roleNames() const override {
    QHash<int, QByteArray> roleNames = m_lstRoleNames;
    int iRoleCount = m_lstRoleNames.keys().last() + 1;
    for (long l = 0;
         l < (m_pDataMemberX ? m_pDataMemberX->count_WithDaoStrategy() : 0);
         ++l) {
      qx::IxDataMember *pDataMember = m_pDataMemberX->get_WithDaoStrategy(l);
      if (!pDataMember) {
        continue;
      }
      if (pDataMember->getKey().contains('|')) {
        QStringList keyParts = pDataMember->getKey().split('|');
        foreach (QString key, keyParts) {
          roleNames.insert(static_cast<int>(iRoleCount), key.toUtf8());
          iRoleCount++;
        }
      }
    }
    roleNames.insert(999, QString("display").toUtf8());
    return roleNames;
  }

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override {
    QVariant variant;
    int iaddtionalRoleCount = 0;
    for (long l = 0;
         l < (m_pDataMemberX ? m_pDataMemberX->count_WithDaoStrategy() : 0);
         ++l) {
      qx::IxDataMember *pDataMember = m_pDataMemberX->get_WithDaoStrategy(l);
      if (!pDataMember) {
        continue;
      }
      if (pDataMember->getKey().contains('|')) {
        QStringList keyParts = pDataMember->getKey().split('|');
        foreach (QString key, keyParts) { iaddtionalRoleCount++; }
      }
    }
    int lrole = role;
    if (role == 999) {
      qx::IxDataMember * pDataMember = nullptr;
      if (index.column() >= iaddtionalRoleCount) {
        pDataMember = m_lstDataMember.value((index.column() - iaddtionalRoleCount) + 1);
      } else {
        pDataMember = m_lstDataMember.value(0);
      }
      void *pItem = getRowItemAsVoidPtr(index.row());
      if (!pDataMember || !pItem) {
        return QVariant("");
      }
      if (pDataMember->getKey().contains('|')) {
        QStringList keyParts = pDataMember->getKey().split('|');
        int keyPart = 0;
        foreach (QString key, keyParts) {
          if (index.column() == keyPart) {
            variant = pDataMember->toVariant(pItem, keyPart);
            return variant;
          }
          keyPart++;
        }
      }
      QVariant value = pDataMember->toVariant(pItem);
      if(!value.isValid()){
          return QVariant("");
      }
      return value;
    }
    if (lrole >= m_lstRoleNames.count()) {
        int iRoleCount = m_lstRoleNames.keys().last();
      for (long l = 0;
           l < (m_pDataMemberX ? m_pDataMemberX->count_WithDaoStrategy() : 0);
           ++l) {
        qx::IxDataMember *pDataMember = m_pDataMemberX->get_WithDaoStrategy(l);
        if (!pDataMember) {
          continue;
        }
        if (pDataMember->getKey().contains('|')) {
          QStringList keyParts = pDataMember->getKey().split('|');
          int keyPart = 0;
          foreach (QString key, keyParts) {
            if (iRoleCount == lrole) {
              void *pItem = getRowItemAsVoidPtr(index.row());
              variant = pDataMember->toVariant(pItem, keyPart);
              return variant;
            } else {
              iRoleCount++;
              keyPart++;
            }
          }
        }
      }
    }
    return qx::IxModel::data(index, role);
  }

  virtual int
  columnCount(const QModelIndex &parent = QModelIndex()) const override {

    QHash<int, QByteArray> roleNames = m_lstRoleNames;
    int iRoleCount = m_lstRoleNames.keys().last() + 1;
    for (long l = 0;
         l < (m_pDataMemberX ? m_pDataMemberX->count_WithDaoStrategy() : 0);
         ++l) {
      qx::IxDataMember *pDataMember = m_pDataMemberX->get_WithDaoStrategy(l);
      if (!pDataMember) {
        continue;
      }
      if (pDataMember->getKey().contains('|')) {
        QStringList keyParts = pDataMember->getKey().split('|');
        foreach (QString key, keyParts) {
          roleNames.insert(static_cast<int>(iRoleCount), key.toUtf8());
          iRoleCount++;
        }
      }
    }
    return static_cast<int>(roleNames.count()-1);
  }
  virtual QModelIndex
  index(int row, int column,
        const QModelIndex &parent = QModelIndex()) const override {


      int iaddtionalRoleCount = 0;
      for (long l = 0;
           l < (m_pDataMemberX ? m_pDataMemberX->count_WithDaoStrategy() : 0);
           ++l) {
        qx::IxDataMember *pDataMember = m_pDataMemberX->get_WithDaoStrategy(l);
        if (!pDataMember) {
          continue;
        }
        if (pDataMember->getKey().contains('|')) {
          QStringList keyParts = pDataMember->getKey().split('|');
          foreach (QString key, keyParts) { iaddtionalRoleCount++; }
        }
      }
      qx::IxDataMember * pDataMember = nullptr;
      if (column > iaddtionalRoleCount) {
        pDataMember = m_lstDataMember.value(column - iaddtionalRoleCount);
      } else {
        pDataMember = m_lstDataMember.value(0);
      }
    return createIndex(row,column,pDataMember);
  }
};
template <typename T1, typename T2, typename KEY_TYPE>
class QxModelServiceBase
    : public qx::QxModelService<T1, T2, QxModelBase<KEY_TYPE>> {

public:
  QxModelServiceBase(QObject *parent = nullptr)
      : qx::QxModelService<T1, T2, QxModelBase<KEY_TYPE>>(parent) {}
  QxModelServiceBase(qx::IxModel *other, QObject *parent = nullptr)
      : qx::QxModelService<T1, T2, QxModelBase<KEY_TYPE>>(other, parent) {}
};

#endif // QXMODELBASE_H
