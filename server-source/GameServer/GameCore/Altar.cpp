#include "stdafx.h"
#include "./Altar.h"
#include "./player.h"

CAltar::CAltar( ALTAR_TYPE etype ):m_Type(etype)
{

}


void CAltar::Init()
{

}

CAltarElem::CAltarElem(CAltar::ALTAR_TYPE CAltarType, ConsortiaElem *pConsortia,PyObject* pAltar,UINT level)
{
	m_LV = level;
	m_pConsortia = pConsortia;
	m_pAltar = pAltar;
  // PyObject* TempConfig = NULL;
 //  PyObject *pyAltar = PyObject_GetAttrString(Module , "Oblation");
 
}

CAltarElem::~CAltarElem()
{
}

bool CAltarElem::UpGrade()
{
	if ( m_LV >= m_pConsortia->iLevel )
	{
		return false;
	}
	PyObject *pFunc = PyObject_GetAttrString(m_pAltar, "LvValues");
	PyObject *pArg  = Py_BuildValue("(i)",m_LV);
	PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
	Py_XDECREF(pArg);

	if ( NULL == AltarRequst )
	{
		Py_XDECREF(AltarRequst);
		PyErr_Print();
		PyErr_Clear();
		return false;
	}								 

	int Money,Bijou;
    Money=Bijou=0;
	if (PyTuple_Check(AltarRequst) == 1)
	{
		 Money = PyInt_AsLong( PyTuple_GetItem(AltarRequst,0) );
		 Bijou = PyInt_AsLong( PyTuple_GetItem(AltarRequst,1) );
		if ( m_pConsortia->lfound < Money || m_pConsortia->lbijou < Bijou || m_pConsortia->iLevel < m_LV  )
			return false;
	}
	else
		return false;

	m_LV++;
    m_pConsortia->lfound -= Money;
	m_pConsortia->lbijou -= Bijou;

	Py_XDECREF(AltarRequst);
	
	return true;
} 

bool CAltarElem::Oblation( CPlayer *pPlayer ,UINT ID,int type)
{
	if( NULL == pPlayer)
		return false;

	//�õ�ȷ���ļ�̳����ű�����
	PyObject *pFuncCon = PyObject_GetAttrString(m_pAltar, "ConValues");
	if ( pFuncCon == NULL  )
	{
		PyErr_Print();
		PyErr_Clear();
		return false;
	}
	PyObject *ConValue = PyObject_CallObject(pFuncCon, 0);
	if ( ConValue == NULL  )
	{
		Py_XDECREF(ConValue);
		PyErr_Print();
		PyErr_Clear();
		return false;
	}

	if (type == 0)  //��������ļ�̳
	{
		if( pPlayer->GetOblationLV( ID ) >= m_LV  )
			return false;

		//ȡ����Ӧ����������(���ṱ��)
		pFuncCon = PyObject_GetAttrString(ConValue, "GetConsume");
		if ( pFuncCon == NULL  )
		{
			PyErr_Print();
			PyErr_Clear();
			return false;
		}

		PyObject *pArg  = Py_BuildValue("(i)",pPlayer->GetOblationLV( ID ) + 1);
		PyObject * pConsume = PyEval_CallObject( pFuncCon, pArg);
		Py_XDECREF(pArg);
		if ( pConsume == NULL  )
		{
			Py_XDECREF(pConsume);
			PyErr_Print();
			PyErr_Clear();
			return false;
		}

		int Consume = PyInt_AsLong(pConsume); 
		int SurpleCon = pPlayer->GetuiUionContribute();
		if (  SurpleCon < Consume )
			return false;
		Py_XDECREF(pConsume);

		//�����������
		//step1:����ǰһ�ȼ���Ч����ֵ 
		PyObject *pFunc = PyObject_GetAttrString(ConValue, "Release");
		if ( pFunc == NULL  )
		{
			PyErr_Print();
			PyErr_Clear();
			return false;
		}

		pArg  = Py_BuildValue("(i,i)",pPlayer->GetOblationLV( ID ),pPlayer);
		PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
		Py_XDECREF(pArg);

		if ( AltarRequst == NULL  )
		{
			Py_XDECREF(AltarRequst);
			PyErr_Print();
			PyErr_Clear();
			return false;
		}

		if ( AltarRequst == Py_True )
		{
			//step2:������̳����
			pFunc = PyObject_GetAttrString(ConValue, "LvUp");
			if ( pFunc == NULL  )
			{
				PyErr_Print();
				PyErr_Clear();
				return false;
			}

			pArg  = Py_BuildValue("(i,i)",pPlayer->GetOblationLV( ID )+1,pPlayer);
			PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
			Py_XDECREF(pArg);
			if ( AltarRequst == NULL  )
			{
				Py_XDECREF(AltarRequst);
				PyErr_Print();
				PyErr_Clear();
				return false;
			}

			if ( AltarRequst == Py_True )
			{
				Py_XDECREF(AltarRequst);
				//��������ȼ���ID������
				pPlayer->UPOblationLV(ID);
				return true;
			}
			else
			{
				Py_XDECREF(AltarRequst);
				return false;
			}
		}
		else
		{
			Py_XDECREF(AltarRequst);
			return false;
		}

	}
	else if ( type == 1 )  //ɾ������ļ�̳
	{
		PyObject *pFunc = PyObject_GetAttrString(ConValue, "Release");
		if ( pFunc == NULL  )
		{
			PyErr_Print();
			PyErr_Clear();
			return false;
		}
		PyObject* pArg  = Py_BuildValue("(i,i)",pPlayer->GetOblationLV( ID ),pPlayer);
		PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
		Py_XDECREF(pArg);
		if ( AltarRequst == NULL  )
		{
			Py_XDECREF(AltarRequst);
			PyErr_Print();
			PyErr_Clear();
			return false;
		}

		Py_XDECREF(AltarRequst);
		if ( AltarRequst == Py_True )
		{

			pPlayer->DelOblation( ID );
			return true;
		}
		else 
		{
			return false;
		}

	}
	else if ( type == 2 ) //�����Ѿ��еļ�̳
	{
		PyObject* pFunc = PyObject_GetAttrString(ConValue, "LoadImpact");
		if ( pFunc == NULL  )
		{
			PyErr_Print();
			PyErr_Clear();
			return false;
		}

		PyObject *pArg  = Py_BuildValue("(i,i)",pPlayer->GetOblationLV( ID ),pPlayer);
		PyObject *AltarRequst =  PyEval_CallObject(pFunc, pArg);
		Py_XDECREF(pArg);
		if ( AltarRequst == NULL  )
		{
			Py_XDECREF(AltarRequst);
			PyErr_Print();
			PyErr_Clear();
			return false;
		}

		Py_XDECREF(AltarRequst);
		if ( AltarRequst == Py_True )
			return true;
		else
			return false;
	}

   Py_XDECREF(ConValue);
	
   return true;
}
