/*******************************************************************************

   Copyright (C) 2011-2014 SequoiaDB Ltd.

   This program is free software: you can redistribute it and/or modify
   it under the term of the GNU Affero General Public License, version 3,
   as published by the Free Software Foundation.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warrenty of
   MARCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with this program. If not, see <http://www.gnu.org/license/>.

   Source File Name = catDCManager.hpp

   Descriptive Name =

   When/how to use: this program may be used on binary and text-formatted
   versions of runtime component. This file contains code logic for
   common functions for coordinator node.

   Dependencies: N/A

   Restrictions: N/A

   Change Activity:
   defect Date        Who Description
   ====== =========== === ==============================================

   Last Changed =     XJH Opt

*******************************************************************************/
#ifndef CAT_DCMANAGER_HPP__
#define CAT_DCMANAGER_HPP__

#include "pmd.hpp"
#include "netDef.hpp"
#include "catDCLogMgr.hpp"
#include "rtnContextBuff.hpp"
#include "dpsLogDef.hpp"
#include "dpsMessageBlock.hpp"

using namespace bson ;

namespace engine
{
   class sdbCatalogueCB ;
   class _SDB_RTNCB ;
   class _dpsLogWrapper ;
   class _SDB_DMSCB ;
   class _clsDCMgr ;
   class _clsDCBaseInfo ;

   /*
      _catDCManager define
   */
   class _catDCManager : public SDBObject
   {
   public:
      _catDCManager() ;
      ~_catDCManager() ;

      catDCLogMgr* getLogMgr() { return _pLogMgr ; }

      INT32 init() ;

      void  attachCB( _pmdEDUCB *cb ) ;
      void  detachCB( _pmdEDUCB *cb ) ;

      INT32 processMsg( const NET_HANDLE &handle, MsgHeader *pMsg ) ;

      INT32 active() ;
      INT32 deactive() ;

      INT32 updateGlobalAddr() ;

      BOOLEAN isDCActive() const ;
      BOOLEAN isImageEnable() const ;
      BOOLEAN groupInImage( UINT32 groupID ) ;
      BOOLEAN groupInImage( const string &groupName ) ;

      void    setImageCommand( BOOLEAN imageCmd ) { _isImageCmd = imageCmd ; }
      BOOLEAN isImageCommand() const { return _isImageCmd ; }

      void    onCommandBegin( MsgHeader *pMsg ) ;
      void    onCommandEnd( MsgHeader *pMsg, INT32 result ) ;

   protected:
      INT32 processCommandMsg( const NET_HANDLE &handle, MsgHeader *pMsg,
                               BOOLEAN writable ) ;
      INT32 processCmdAlterImage( const NET_HANDLE &handle,
                                  const CHAR *pQuery,
                                  rtnContextBuf &ctxBuff ) ;

   protected:
      INT32 processCmdAttachImage( const NET_HANDLE &handle,
                                   _clsDCMgr *pDCMgr,
                                   const BSONObj &objQuery,
                                   BSONObjBuilder &retObjBuilder ) ;
      INT32 processCmdEnableImage( const NET_HANDLE &handle,
                                   _clsDCMgr *pDCMgr,
                                   const BSONObj &objQuery,
                                   BSONObjBuilder &retObjBuilder ) ;
      INT32 processCmdDetachImage( const NET_HANDLE &handle,
                                   _clsDCMgr *pDCMgr,
                                   const BSONObj &objQuery,
                                   BSONObjBuilder &retObjBuilder ) ;
      INT32 processCmdDisableImage( const NET_HANDLE &handle,
                                    _clsDCMgr *pDCMgr,
                                    const BSONObj &objQuery,
                                    BSONObjBuilder &retObjBuilder) ;
      INT32 processCmdActiveImage( const NET_HANDLE &handle,
                                   _clsDCMgr *pDCMgr,
                                   const BSONObj &objQuery,
                                   BSONObjBuilder &retObjBuilder ) ;
      INT32 processCmdDeactiveImage( const NET_HANDLE &handle,
                                     _clsDCMgr *pDCMgr,
                                     const BSONObj &objQuery,
                                     BSONObjBuilder &retObjBuilder ) ;

   protected:
      void  _fillRspHeader( MsgHeader *rspMsg, const MsgHeader *reqMsg ) ;

      INT32 _mapData2DCMgr( _clsDCMgr *pDCMgr ) ;

      void  _dcBaseInfoGroups2Obj( _clsDCBaseInfo *pInfo,
                                   BSONObjBuilder &builder,
                                   const CHAR *pFieldName ) ;

      BOOLEAN _isAddrConflictWithSelf( const string &addr ) ;

      INT32   _checkGroupsValid( map< string, string > &mapGroups ) ;

   private:
      INT16 _majoritySize() ;
      INT32 _updateGlobalInfo() ;
      INT32 _updateImageInfo() ;

   private:
      _SDB_DMSCB                 *_pDmsCB;
      _dpsLogWrapper             *_pDpsCB;
      _SDB_RTNCB                 *_pRtnCB;
      sdbCatalogueCB             *_pCatCB;
      pmdEDUCB                   *_pEduCB;

      _clsDCMgr                  *_pDCMgr ;
      _clsDCBaseInfo             *_pDCBaseInfo ;
      _catDCLogMgr               *_pLogMgr ;

      BOOLEAN                    _isImageCmd ;
      DPS_LSN                    _lsn ;
      _dpsMessageBlock           _mb ;

   } ;
   typedef _catDCManager catDCManager ;

}

#endif // CAT_DCMANAGER_HPP__

