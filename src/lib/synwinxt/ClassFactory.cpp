/*
 * synergy -- mouse and keyboard sharing utility
 * Copyright (C) 2013 Bolton Software Ltd.
 * 
 * This package is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * found in the file COPYING that should have accompanied this file.
 * 
 * This package is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "synwinxt/ClassFactory.h"
#include "synwinxt/DataHandlerExtension.h"

#include <Shlwapi.h>

extern LONG g_refCount;
extern void log(const char *str, ...);

CClassFactory::CClassFactory() :
	m_refCount(1)
{
	log("> CClassFactory::ctor, g_refCount=%d", g_refCount);
	InterlockedIncrement(&g_refCount);
	log("< CClassFactory::ctor, g_refCount=%d", g_refCount);
}

CClassFactory::~CClassFactory()
{
	log("> CClassFactory::dtor, g_refCount=%d", g_refCount);
	InterlockedDecrement(&g_refCount);
	log("< CClassFactory::dtor, g_refCount=%d", g_refCount);
}

HRESULT STDMETHODCALLTYPE
CClassFactory::QueryInterface(REFIID riid, void **ppvObject)
{
	log("> CClassFactory::QueryInterface");
	static const QITAB qit[] = 
	{
		QITABENT(CClassFactory, IClassFactory),
		{ 0 },
	};
	HRESULT hr = QISearch(this, qit, riid, ppvObject);
	
	log("< CClassFactory::QueryInterface, hr=%d", hr);
	return hr;
}

ULONG STDMETHODCALLTYPE
CClassFactory::AddRef()
{
	log("> CClassFactory::AddRef, m_refCount=%d", m_refCount);
	LONG r = InterlockedIncrement(&m_refCount);
	log("< CClassFactory::AddRef, r=%d, m_refCount=%d", r, m_refCount);
	return r;
}

ULONG STDMETHODCALLTYPE
CClassFactory::Release()
{
	log("> CClassFactory::Release, m_refCount=%d", m_refCount);
	LONG r = InterlockedDecrement(&m_refCount);
	if (r == 0) {
		delete this;
	}

	log("< CClassFactory::Release, r=%d", r);
	return r;
}

HRESULT STDMETHODCALLTYPE
CClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppvObject)
{
	log("> CClassFactory::CreateInstance");
	if (pUnkOuter != NULL) {
		return CLASS_E_NOAGGREGATION;
	}

	CDataHandlerExtension* pExtension = new CDataHandlerExtension();
	HRESULT hr = pExtension->QueryInterface(riid, ppvObject);
	if (FAILED(hr)) {
		delete pExtension;
	}
	
	log("< CClassFactory::CreateInstance, hr=%d", hr);
	return hr;
}

HRESULT STDMETHODCALLTYPE
CClassFactory::LockServer(BOOL fLock)
{
	log("> CClassFactory::LockServer, g_refCount=%d", g_refCount);
	if (fLock) {
		InterlockedIncrement(&g_refCount);
	}
	else {
		InterlockedDecrement(&g_refCount);
	}
	log("< CClassFactory::LockServer, g_refCount=%d", g_refCount);
	return S_OK;
}