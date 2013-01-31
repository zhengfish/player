#include "MySerialize.h"
#include "customMsg.h"
#include "Util1.h"
#include "MyLib.h"
//#include "PlayList.h"
#include "globalStuffs.h"

template <class T>
int Serialize (FILE *pFile,T t)
{
	return 0;
};

template <>
int Serialize (FILE *pFile,UINT t)
{
	fwrite(&t,1,4,pFile);
	return 4;
};


template <>
int Serialize (FILE *pFile,std::tstring t)
{
	const TCHAR* str=t.c_str();
	int size=t.length()*sizeof(TCHAR);
	int totalSize=size+4;

	fwrite(&totalSize,1,4,pFile);
	fwrite(str,1,size,pFile);
	return totalSize;
};

template <>
int Serialize (FILE *pFile,int t)
{
	fwrite(&t,1,4,pFile);
	return 4;
};

//-----------------------------------------

template <class T>
int ReSerialize (FILE *pFile,T *t)
{

	return 0;
};

template <>
int ReSerialize (FILE *pFile,std::tstring *str)
{
	int size;
	fread(&size,1,4,pFile);

	//string len is 0
	if(size-4<0)
	{
		MessageBox(MyLib::GetMain(),_T("Parse File Failed,Invalide Data"),_T(""),MB_OK);
		return 0;
	}
	else if (size-4==0)
	{
		*str=_T("");
	}
	else{
		int len=(size-4)/sizeof(TCHAR);
		TCHAR* pStr=new TCHAR[len];
		fread(pStr,sizeof(TCHAR),len,pFile);

		std::tstring tmp(pStr,pStr+len);
		delete[] pStr;
		(*str)=tmp;
	}

	return size;
};


template <>
int ReSerialize (FILE *pFile,int *pVal)
{
	fread(pVal,4,1,pFile);
	return 4;
};

template <>
int ReSerialize (FILE *pFile,UINT *pVal)
{
	fread(pVal,4,1,pFile);
	return 4;
};



int MyLib::SerializeB(FILE *pFile)
{
	int size=0;
	// 	list<PlayList>::iterator i;
	// 	for (i=m_playLists.begin();i!=m_playLists.end();++i)
	// 	{
	// 		size+=(*i).Serialize(out);
	// 	}
	size+=ActivePlaylist()->Serialize(pFile);
	return size;
}

int MyLib::ReSerialize(FILE *pFile)
{
	int totalSize=0;
	fread(&totalSize,1,4,pFile);

	int size=ActivePlaylist()->ReSerialize(pFile);

	return size;
}

int PlayList::SerializeB(FILE *pFile)
{
	int size=0;

	//m_playlistName Serialize
	size+=::Serialize(pFile,m_playlistName);

	//m_songList Serialize
	_songContainer::iterator i;
	for (i=m_songList.begin();i!=m_songList.end();++i){
		size+=(*i).GetFileTrack()->Serialize(pFile);
	}

	return size;
}

int PlayList::ReSerialize(FILE *pFile)
{
	int size=0;
	int totalSize=0;
	int playlistnameSize=0;
	fread(&totalSize,1,4,pFile);

	playlistnameSize=::ReSerialize(pFile,&m_playlistName);
	size+=playlistnameSize;

	while(size<totalSize-playlistnameSize){
		PlayListItem item(this);
		size+=item.ReSerialize(pFile);
		item.SetIndex(m_songList.size());
		m_songList.push_back(item);
	}


	return size;
}


int FileTrack::SerializeB(FILE *pFile)
{
	int size=0;
	size+=::Serialize(pFile,url);
	size+=::Serialize(pFile,playCount);
	size+=::Serialize(pFile,starLvl);
	size+=::Serialize(pFile,title);
	size+=::Serialize(pFile,artist);
	size+=::Serialize(pFile,album);
	size+=::Serialize(pFile,genre);
	size+=::Serialize(pFile,year);
	return size;
}

int FileTrack::ReSerialize(FILE *pFile)
{
	int size=0;

	int totalSize=0;
	fread(&totalSize,1,4,pFile);
	size+=4;

	size+=::ReSerialize(pFile,&url);
	size+=::ReSerialize(pFile,&playCount);
	size+=::ReSerialize(pFile,&starLvl);
	size+=::ReSerialize(pFile,&title);
	size+=::ReSerialize(pFile,&artist);
	size+=::ReSerialize(pFile,&album);
	size+=::ReSerialize(pFile,&genre);
	size+=::ReSerialize(pFile,&year);
	return size;
}


BOOL MyLib::SavePlaylist(PlayList *pl,LPTSTR filepath)
{
	BOOL result=FALSE;
	FILE * pFile;
	pFile = _tfopen((LPCTSTR)filepath , _T("wb") );
	if (pFile!=NULL){
		result=pl->Serialize(pFile);
		pl->m_saveLocation=filepath;
		fclose (pFile);
	}

	return result;
}

PlayList* MyLib::LoadPlaylist(LPTSTR filepath)
{
	PlayList *playlist=NULL;
	BOOL result=FALSE;
	FILE * pFile;

	pFile = _tfopen ((LPCTSTR)filepath, _T("rb") );
	if (pFile!=NULL){
		playlist=new PlayList;
		playlist->m_saveLocation=filepath;
		result=playlist->ReSerialize(pFile);
		m_playLists.push_back(playlist);
		SdMsg(WM_PL_TRACKNUM_CHANGED,TRUE,(WPARAM)playlist,(LPARAM)playlist->GetItemCount());
		fclose (pFile);
	}


	SetActivePlaylist(playlist);
	return playlist;
}

BOOL MyLib::SaveCoreCfg()
{
	FILE * pFile;
	ChangeCurDir2ModulePath();
	pFile = _tfopen( CFGFILENAME , _T("wb") );
	if (pFile!=NULL)
	{	
		//playlist section
		int len=m_playLists.size();
		::Serialize(pFile,len);
		
		MyLib::PLList::iterator i;
		for (i=m_playLists.begin();i!=m_playLists.end();++i)
			::Serialize(pFile,(*i)->m_saveLocation);
		
		//lrc section
		len=lrcDirs.size();
		::Serialize(pFile,len);

		vector<std::tstring>::iterator k;
		for (k=lrcDirs.begin();k!=lrcDirs.end();++k)
			::Serialize<>(pFile,*k);
		
		
		fclose (pFile);
	}


	return TRUE;
}



BOOL MyLib::LoadCoreCfg()
{
	FILE * pFile;
	ChangeCurDir2ModulePath();
	pFile = _tfopen( CFGFILENAME, _T("rb") );
	if (pFile!=NULL)
	{
		//playlists
		int size=0;

		::ReSerialize(pFile,&size);
		while (size) 
		{
			std::tstring playlistLocation;
			::ReSerialize(pFile,&playlistLocation);
			MyLib::shared()->LoadPlaylist(const_cast<LPTSTR>(playlistLocation.c_str()));
			
			size--;
		} 

		//lrc dir list
		size=0;
		::ReSerialize(pFile,&size);
		 while (size--)
		{
			std::tstring lrcDir;

			::ReSerialize(pFile,&lrcDir);
			if(!lrcDir.empty())
				lrcDirs.push_back(lrcDir);
		}

		fclose (pFile);
	}

	return TRUE;
}