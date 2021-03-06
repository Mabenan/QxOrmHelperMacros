#ifndef QXORMHELPERMACROS_EXPORT_H
#define QXORMHELPERMACROS_EXPORT_H


#ifdef _BUILDING_QX_ORM_HELPER_MACROS
#define QX_ORM_HELPER_MACROS_DLL_EXPORT QX_DLL_EXPORT_HELPER
#else // _BUILDING_QX_ORM_HELPER_MACROS
#define QX_ORM_HELPER_MACROS_DLL_EXPORT QX_DLL_IMPORT_HELPER
#endif // _BUILDING_QX_ORM_HELPER_MACROS

#ifdef _BUILDING_QX_ORM_HELPER_MACROS
#define REGISTER_HPP_QX_ORM_HELPER_MACROS QX_REGISTER_HPP_EXPORT_DLL
#define REGISTER_CPP_QX_ORM_HELPER_MACROS QX_REGISTER_CPP_EXPORT_DLL
#define REGISTER_COMPLEX_HPP_QX_ORM_HELPER_MACROS QX_REGISTER_COMPLEX_CLASS_NAME_HPP_EXPORT_DLL
#define REGISTER_COMPLEX_CPP_QX_ORM_HELPER_MACROS QX_REGISTER_COMPLEX_CLASS_NAME_CPP_EXPORT_DLL
#else // _BUILDING_QX_ORM_HELPER_MACROS
#define REGISTER_HPP_QX_ORM_HELPER_MACROS QX_REGISTER_HPP_IMPORT_DLL
#define REGISTER_CPP_QX_ORM_HELPER_MACROS QX_REGISTER_CPP_IMPORT_DLL
#define REGISTER_COMPLEX_HPP_QX_ORM_HELPER_MACROS QX_REGISTER_COMPLEX_CLASS_NAME_HPP_IMPORT_DLL
#define REGISTER_COMPLEX_CPP_QX_ORM_HELPER_MACROS QX_REGISTER_COMPLEX_CLASS_NAME_CPP_IMPORT_DLL
#endif // _BUILDING_QX_ORM_HELPER_MACROS,

#endif // QXORMHELPERMACROS_EXPORT_H
