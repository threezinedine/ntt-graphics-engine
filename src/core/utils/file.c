#include "engine/core/utils/file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct ntt_File* ntt_LoadFile(const char* path, enum ntt_FileMode mode)
{
	struct ntt_File* file = (struct ntt_File*)malloc(sizeof(struct ntt_File));
	if (!file)
	{
		return NULL;
	}
	memcpy(file->path, path, strlen(path) + 1);
	file->mode = mode;
	const char* modeStr = "rb";
	if (mode == NTT_FILE_MODE_WRITE)
	{
		modeStr = "w";
	}
	else if (mode == NTT_FILE_MODE_WRITE_BINARY)
	{
		modeStr = "wb";
	}
	else if (mode == NTT_FILE_MODE_APPEND)
	{
		modeStr = "ab";
	}
	FILE* fp	   = fopen(path, modeStr);
	file->handle   = fp;
	file->size	   = fp ? (usize)fseek(fp, 0, SEEK_END), ftell(fp) : 0;
	file->isLoaded = fp != NULL;
	return file;
}

void ntt_ReadFile(struct ntt_File* file, char* pBuffer)
{
	if (!file || !pBuffer || !file->isLoaded)
	{
		return;
	}

	FILE* fp = (FILE*)file->handle;
	fseek(fp, 0, SEEK_SET);
	fread(pBuffer, 1, file->size, fp);
	pBuffer[file->size] = '\0';
}

void ntt_WriteFile(struct ntt_File* file, const void* pBuffer, usize size, b8 append)
{
	if (!file || !pBuffer || !file->isLoaded)
	{
		return;
	}

	FILE* fp = (FILE*)file->handle;
	fseek(fp, 0, append ? SEEK_END : SEEK_SET);
	fwrite(pBuffer, 1, size, fp);
	file->size += size;
}

void ntt_UnloadFile(struct ntt_File* file)
{
	if (!file || !file->isLoaded)
	{
		return;
	}

	FILE* fp = (FILE*)file->handle;
	fclose(fp);
	file->isLoaded = 0;
	free(file);
}