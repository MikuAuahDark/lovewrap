/**
 * Copyright (c) 2040 Dark Energy Processor
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the authors be held liable for any damages
 * arising from the use of this software.
 * 
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

// lovewrap
#include "LOVEWrap.h"

namespace lovewrap
{
namespace filesystem
{

using namespace love::filesystem;

void append(const std::string &filename, const void *data, int64_t size)
{
	getInstance()->append(filename.c_str(), data, size);
}

bool areSymlinksEnabled()
{
	return getInstance()->areSymlinksEnabled();
}

std::string getSaveDirectory()
{
	return getInstance()->getSaveDirectory();
}

FileData *newFileData(const std::string &path)
{
	love::StrongRef<love::filesystem::File> f;
	f.set(getInstance()->newFile(path.c_str()), love::Acquire::NORETAIN);

	if (f->open(File::MODE_READ))
		return f->read();

	throw love::Exception("Could not open file.");
}

FileData *newFileData(const void *contents, size_t len, const std::string &filename)
{
	return getInstance()->newFileData(contents, len, filename.c_str());
}

FileData *newFileData(const love::Data *data, const std::string &filename)
{
	return getInstance()->newFileData(data->getData(), data->getSize(), filename.c_str());
}

} // filesystem
} // love
