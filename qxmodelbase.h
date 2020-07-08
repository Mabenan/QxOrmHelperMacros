#ifndef QXMODELBASE_H
#define QXMODELBASE_H

#include <QxModelView.h>
#include <QxModelView/IxModel.h>
#include <QxOrm.h>

class QxModelBase : public qx::IxModel {
public:
  QxModelBase(QObject *parent = nullptr) : qx::IxModel(parent) {}

public:
  QHash<int, QByteArray> roleNames() const override {
    QHash<int, QByteArray> roleNames = m_lstRoleNames;
    int iRoleCount = m_lstRoleNames.keys().last() + 1;
    for (long l = 0; l < (m_pDataMemberX ? m_pDataMemberX->count() : 0); ++l) {
      qx::IxDataMember *pDataMember = m_pDataMemberX->get(l);
      if (!pDataMember) {
        continue;
      }
      if (pDataMember->getKey().contains('|') &&
          pDataMember->getIsPrimaryKey()) {
        QStringList keyParts = pDataMember->getKey().split('|');
        foreach (QString key, keyParts) {
          roleNames.insert(static_cast<int>(iRoleCount), key.toUtf8());
          iRoleCount++;
        }
      }
    }
    roleNames.insert(Qt::DisplayRole, QString("display").toUtf8());
    roleNames.insert(Qt::EditRole, QString("edit").toUtf8());
    return roleNames;
  }

  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole) override {
    if (!index.isValid()) {
      return false;
    }
    int iaddtionalRoleCount = 0;
    for (long l = 0; l < (m_pDataMemberX ? m_pDataMemberX->count() : 0); ++l) {
      qx::IxDataMember *pDataMember = m_pDataMemberX->get(l);
      if (!pDataMember) {
        continue;
      }
      if (pDataMember->getKey().contains('|') &&
          pDataMember->getIsPrimaryKey()) {
        QStringList keyParts = pDataMember->getKey().split('|');
        foreach (QString key, keyParts) { iaddtionalRoleCount++; }
      }
    }
    if (role == Qt::EditRole) {

      qx::IxDataMember *pDataMember = nullptr;
      if (iaddtionalRoleCount > 0) {
        if (index.column() >= iaddtionalRoleCount) {
          pDataMember =
              m_lstDataMember.value((index.column() - iaddtionalRoleCount) + 1);
        } else {
          pDataMember = m_lstDataMember.value(0);
        }
      } else {
        pDataMember = m_lstDataMember.value(index.column());
      }

      if (pDataMember->getKey().contains("|")) {
        return false;
      }
      bool bDirtyRow = isDirtyRow(index.row());
      void *pItem = getRowItemAsVoidPtr(index.row());
      if (!pDataMember || !pItem) {
        return false;
      }
      QVariant vCurrentValue = pDataMember->toVariant(pItem);
      if (vCurrentValue == value) {
        return true;
      }
      qx_bool bSetData = pDataMember->fromVariant(pItem, value);
      if (bSetData && (m_eAutoUpdateDatabase ==
                       qx::IxModel::e_auto_update_on_field_change)) {
        qxSaveRowData(index.row(), (QStringList() << pDataMember->getKey()));
        if (!m_lastError.isValid() && bDirtyRow) {
          insertDirtyRowToModel();
        }
        if (m_lastError.isValid()) {
          if (!bDirtyRow) {
            qDebug(
                "[QxOrm] qx::IxModel::setData() : %s",
                "an error occurred saving value in database (more details with "
                "'getLastError()' method), so previous value has been "
                "restored");
            pDataMember->fromVariant(pItem, vCurrentValue);
            return false;
          }
        } else if (pDataMember->hasSqlRelation()) {
          qxFetchRow(index.row(), (QStringList()
                                   << pDataMember->getSqlRelation()->getKey()));
        }
      }
      if (bSetData) {
        raiseEvent_dataChanged(index, index);
      }
      return bSetData;
    } else if (role >= (Qt::UserRole + 1)) {
      QModelIndex idx =
          this->index(index.row(), (role - Qt::UserRole - 1), QModelIndex());
      return setData(idx, value, Qt::EditRole);
    }
    return false;
  }

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override {
    QVariant variant;
    int iaddtionalRoleCount = 0;
    for (long l = 0; l < (m_pDataMemberX ? m_pDataMemberX->count() : 0); ++l) {
      qx::IxDataMember *pDataMember = m_pDataMemberX->get(l);
      if (!pDataMember) {
        continue;
      }
      if (pDataMember->getKey().contains('|') &&
          pDataMember->getIsPrimaryKey()) {
        QStringList keyParts = pDataMember->getKey().split('|');
        foreach (QString key, keyParts) { iaddtionalRoleCount++; }
      }
    }
    int lrole = role;
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
      qx::IxDataMember *pDataMember = nullptr;
      if (iaddtionalRoleCount > 0) {
        if (index.column() >= iaddtionalRoleCount) {
          pDataMember =
              m_lstDataMember.value((index.column() - iaddtionalRoleCount) + 1);
        } else {
          pDataMember = m_lstDataMember.value(0);
        }
      } else {
        pDataMember = m_lstDataMember.value(index.column());
      }
      void *pItem = getRowItemAsVoidPtr(index.row());
      if (!pDataMember || !pItem) {
        return QVariant("");
      }
      if (pDataMember->getKey().contains('|') &&
          pDataMember->getIsPrimaryKey()) {
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
      if (!value.isValid()) {
        return QVariant("");
      }
      return value;
    }
    if (lrole >= m_lstRoleNames.count()) {
      int iRoleCount = m_lstRoleNames.keys().last();
      for (long l = 0; l < (m_pDataMemberX ? m_pDataMemberX->count() : 0);
           ++l) {
        qx::IxDataMember *pDataMember = m_pDataMemberX->get(l);
        if (!pDataMember) {
          continue;
        }
        if (pDataMember->getKey().contains('|') &&
            pDataMember->getIsPrimaryKey()) {
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
    for (long l = 0; l < (m_pDataMemberX ? m_pDataMemberX->count() : 0); ++l) {
      qx::IxDataMember *pDataMember = m_pDataMemberX->get(l);
      if (!pDataMember) {
        continue;
      }

      if (pDataMember->getKey().contains('|') &&
          pDataMember->getIsPrimaryKey()) {
        QStringList keyParts = pDataMember->getKey().split('|');
        foreach (QString key, keyParts) {
          roleNames.insert(static_cast<int>(iRoleCount), key.toUtf8());
          iRoleCount++;
        }
      }
    }
    return static_cast<int>(roleNames.count());
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
    qx::IxDataMember *pDataMember = nullptr;
    if (column > iaddtionalRoleCount) {
      pDataMember = m_lstDataMember.value(column - iaddtionalRoleCount);
    } else {
      pDataMember = m_lstDataMember.value(0);
    }
    return createIndex(row, column, pDataMember);
  }
};
template <typename T1, typename T2>
class QxModelServiceBase : public qx::QxModelService<T1, T2, QxModelBase> {

public:
  QxModelServiceBase(QObject *parent = nullptr)
      : qx::QxModelService<T1, T2, QxModelBase>(parent) {}
  QxModelServiceBase(qx::IxModel *other, QObject *parent = nullptr)
      : qx::QxModelService<T1, T2, QxModelBase>(other, parent) {}
};

#endif // QXMODELBASE_H
