/*
 * The Initial Developer of the Original Code is International
 * Business Machines Corporation. Portions created by IBM
 * Corporation are Copyright (C) 2005 International Business
 * Machines Corporation. All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the Common Public License as published by
 * IBM Corporation; either version 1 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * Common Public License for more details.
 *
 * You should have received a copy of the Common Public License
 * along with this program; if not, a copy can be viewed at
 * http://www.opensource.org/licenses/cpl1.0.php.
 */

/* (C) COPYRIGHT International Business Machines Corp. 2001, 2002, 2005 */


// File:  data_obj.c
//
// Functions contained within:
//
//    data_object_check_required_attributes
//    data_object_set_default_attributes
//    data_object_validate_attribute
//

//#include <windows.h>

#include <pthread.h>
#include <stdlib.h>

  #include <string.h>  // for memcmp() et al

#include "pkcs11/pkcs11types.h"
#include "pkcs11/stdll.h"
#include "defs.h"
#include "host_defs.h"
#include "h_extern.h"


// data_object_check_required_attributes()
//
CK_RV
data_object_check_required_attributes( TEMPLATE *tmpl, CK_ULONG mode )
{
   // CKO_DATA has no required attributes
   //

   return template_check_required_base_attributes( tmpl, mode );
}


// data_object_set_default_attributes()
//
// Set the default attributes for data objects:
//
//    CKA_APPLICATION :  empty string
//    CKA_VALUE       :  empty byte array
//
CK_RV
data_object_set_default_attributes( TEMPLATE *tmpl, CK_ULONG mode )
{
   CK_ATTRIBUTE    *class_attr = NULL;
   CK_ATTRIBUTE    *app_attr   = NULL;
   CK_ATTRIBUTE    *value_attr = NULL;

   // satisfy the compiler
   //
   if (mode)
      app_attr = NULL;

   // add the default CKO_DATA attributes
   //
   class_attr = (CK_ATTRIBUTE *)malloc( sizeof(CK_ATTRIBUTE) + sizeof(CK_OBJECT_CLASS) );
   app_attr   = (CK_ATTRIBUTE *)malloc( sizeof(CK_ATTRIBUTE) );
   value_attr = (CK_ATTRIBUTE *)malloc( sizeof(CK_ATTRIBUTE) );

   if (!class_attr || !app_attr || !value_attr) {
      if (class_attr) free( class_attr );
      if (app_attr)   free( app_attr );
      if (value_attr) free( value_attr );
      st_err_log(1, __FILE__, __LINE__);
      return CKR_HOST_MEMORY;
   }

   app_attr->type           = CKA_APPLICATION;
   app_attr->ulValueLen     = 0;  // empty string
   app_attr->pValue         = NULL;

   value_attr->type         = CKA_VALUE;
   value_attr->ulValueLen   = 0;  // empty byte array
   value_attr->pValue       = NULL;

   class_attr->type         = CKA_CLASS;
   class_attr->ulValueLen   = sizeof(CK_OBJECT_CLASS);
   class_attr->pValue       = (CK_BYTE *)class_attr + sizeof(CK_ATTRIBUTE);
   *(CK_OBJECT_CLASS *)class_attr->pValue = CKO_DATA;

   template_update_attribute( tmpl, class_attr );
   template_update_attribute( tmpl, app_attr   );
   template_update_attribute( tmpl, value_attr );

   return CKR_OK;
}


// data_object_validate_attribute()
//
// Determine whether a CKO_DATA object's attribute are valid.
//
CK_RV
data_object_validate_attribute( TEMPLATE *tmpl, CK_ATTRIBUTE *attr, CK_ULONG mode )
{
   if (!attr){
      st_err_log(4, __FILE__, __LINE__, __FUNCTION__);
      return CKR_FUNCTION_FAILED;
   }
   switch (attr->type) {
       case CKA_APPLICATION:
       case CKA_VALUE:
          return CKR_OK;

       default:
          return template_validate_base_attribute( tmpl, attr, mode );
    }

   return CKR_OK;
}