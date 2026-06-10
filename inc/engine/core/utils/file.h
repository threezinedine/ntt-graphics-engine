#ifndef _FILE_H_
#define _FILE_H_

#include "engine/core/types.h"

/**
 * The file module is used for loading and saving files, which is used for reading and writing data to the disk. The
 *      file module provides a simple and consistent interface for file operations across different platforms.
 */
enum ntt_FileMode
{
	NTT_FILE_MODE_READ,
	NTT_FILE_MODE_WRITE,
	NTT_FILE_MODE_WRITE_BINARY,
	NTT_FILE_MODE_APPEND
};

/**
 * The instance of a file in the whole engine, which contains the metadata of that file, the content of the file
 *      be not here.
 */
struct ntt_File
{
	/**
	 * The custom handle for file operations, which depends on the operating system.
	 */
	void*			  handle;
	enum ntt_FileMode mode;
	char			  path[256];
	usize			  size;
	/**
	 * At the default the file is always unloaded, after `ntt_LoadFile` is called, the file will be loaded and this flag
	 *      will be set to true if the file is loaded successfully, otherwise it will be set to false. After
	 *      `ntt_UnloadFile` is called, the file will be unloaded and this flag will be set to false.
	 */
	b8 isLoaded;
};

/**
 * Open the file, load the metadata for later use, the content is not loadded yet.
 *
 * @return The pointer to the file instance, which contains the metadata of that file, the content of the file be not
 */
struct ntt_File* ntt_LoadFile(const char* path, enum ntt_FileMode mode);

/**
 * Read the content of the file into the buffer (manually allocated by the caller).
 *
 * @param file The pointer to the file instance, which contains the metadata of that file.
 * @param pBuffer The pointer to the buffer, which is used for storing the content of the file.
 */
void ntt_ReadFile(struct ntt_File* file, char* pBuffer);

/**
 * Write the content of the buffer into the file, which is used for saving the file. The behavior of this function
 *      depends on the specific file implementation.
 *
 * @param file The pointer to the file instance, which contains the metadata of that file.
 * @param pBuffer The pointer to the buffer, which contains the content to be written to the file.
 * @param size The size of the content to be written to the file.
 * @param append Whether to append the content to the file or overwrite it.
 */
void ntt_WriteFile(struct ntt_File* file, const void* pBuffer, usize size, b8 append);

/**
 * Unload the file, which is used for releasing the resources of the file. The behavior of this function depends on the
 *      specific file implementation.
 *
 * @param file The pointer to the file instance, which contains the metadata of that file.
 */
void ntt_UnloadFile(struct ntt_File* file);

#endif /* _FILE_H_ */