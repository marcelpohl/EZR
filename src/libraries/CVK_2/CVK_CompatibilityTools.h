#ifndef __CVK_COMPATIBILITY_TOOLS_H
#define __CVK_COMPATIBILITY_TOOLS_H

#include "CVK_Defs.h"

namespace CVK 
{
	/**
	 * Use OpenGL Core Profile if using Apple
	 */
	void useOpenGL33CoreProfile();
	void useOpenGL45CoreProfile();
	/**
	 * check the compatibility of the hardware by giving useful output
	 */
	void checkCompatibility();
}

#endif /* __CVK_COMPATIBILITY_TOOLS_H */
