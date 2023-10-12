#include "FileManager.h"

#ifdef _WIN32
#include <io.h>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <Windows.h>
const int CREATE_PMODE = S_IREAD | S_IWRITE;
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#define O_BINARY (0)
const int CREATE_PMODE = S_IRWXU | S_IRWXG | S_IRWXO;
#endif

int g_openfiles = 0;
FileAutoClose::FileAutoClose(const std::string &path, int flags)
{
	m_FP = ::open(path.c_str(), flags, CREATE_PMODE);
	if(m_FP < 0)
	{
		int err = errno;
		if(err != 2)
		{
			err = err;
		}
	}
	else
	{
		g_openfiles++;
		if(g_openfiles > 8)
		{
			int aaa = 0;
		}
	}
}
FileAutoClose::FileAutoClose(const char *path, int flags)
{
	m_FP = open(path, flags, CREATE_PMODE);
	if(m_FP < 0)
	{
		int err = errno;
		if(err != 2)
		{
			err = err;
		}
	}
	else
	{
		g_openfiles++;
		if(g_openfiles > 8)
		{
			int aaa = 0;
		}
	}
}
FileAutoClose::~FileAutoClose()
{
	close();
}

bool FileAutoClose::flush()
{
	if(m_FP >= 0)
	{
		return syncFile(m_FP);
	}

	return false;
}

bool FileAutoClose::syncFile(int fp)
{
#ifdef _WIN32
	auto p = _get_osfhandle(fp);
	return FlushFileBuffers((HANDLE)p);
#else
	return fsync(m_FP);
#endif
}

void FileAutoClose::close()
{
	if(m_FP >= 0)
	{
		::close(m_FP);
		m_FP = -1;

		g_openfiles--;
	}
}

bool FileAutoClose::isNull()
{
	return m_FP<0;
}

bool FileAutoClose::read(void *buf, int nbytes)
{
	int n;
	if((n=::read(m_FP, buf, nbytes)) == nbytes) return true;
	else
	{
		int err = errno;
		if(err != 2)
		{
			err = err;
		}
		return false;
	}
}

bool FileAutoClose::write(const void *buf, int nbytes)
{
	int n;
	if((n=::write(m_FP, buf, nbytes)) == nbytes) return true;
	else
	{
		int err = errno;
		return false;
	}
}

int FileAutoClose::fileSize()
{
	int s = lseek(m_FP, 0, SEEK_END);
	if(s < 0) return 0;
	else return s;
}

int FileAutoClose::tell()
{
	return ::lseek(m_FP, 0, SEEK_CUR);
}

bool FileAutoClose::seek(int offset, int pos)
{
	if(lseek(m_FP, offset, pos) >= 0) return true;
	else
	{
		int err = errno;
		return false;
	}
}

FileManager *g_fileManager = FileManager::GetInstance();
FileManager::FileManager(void)
{
	m_sRootPath = "E:/Workspace/HelloMyCpp/bin/";
}

bool FileManager::renameFile(const char *oldname, const char *newname)
{
	for(int i=0; i<5; i++) //try 5 times
	{
		if(rename(oldname, newname) == 0) return true;

#ifdef _WIN32
		DeleteFileA(newname);
		if(rename(oldname, newname) == 0) return true;
#endif
		::sleep(100);
	}
	return false;
}

void* FileManager::ReadWholeFile(const char *path, int &datalen)
{
	std::string fullpath(m_sRootPath+path);

	FileAutoClose fp(fullpath, O_RDONLY|O_BINARY);
	if (fp.isNull()) return NULL;

	datalen = fp.fileSize();
	if(datalen == 0) return NULL;

	void *buf = malloc(datalen);
	if(buf == NULL) return NULL;

	fp.seek(0, SEEK_SET);
	if(!fp.read(buf, datalen))
	{
		free(buf);
		return NULL;
	}
	return buf;
}

bool FileManager::WriteWholeFile(const char *path, const void *data, int datalen, bool safely/*=false*/)
{
	if(safely)
	{
		char tmppath[256];
		sprintf(tmppath, "%s.tmp", path);
		std::string fullpath(m_sRootPath+path);

		FileAutoClose fp(fullpath, O_CREAT|O_WRONLY|O_TRUNC|O_BINARY);
		if(fp.isNull()) return false;

		if(!fp.write(data, datalen)) return false;

		fp.flush();
		fp.close();

		this->renameFile(tmppath, path);

		return true;
	}
	else
	{
		std::string fullpath(path);

		FileAutoClose fp(fullpath, O_CREAT|O_WRONLY|O_TRUNC|O_BINARY);
		if(fp.isNull()) return false;

		return fp.write(data, datalen);
	}
}
