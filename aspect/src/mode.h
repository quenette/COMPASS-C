/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**
** <copyright>
**
** <license>
**
** $Id$
**
**~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef __Compass_mode_h__
#define __Compass_mode_h__

	/** Mimic the behaviour of a static aspect model within the code. In this mode, as some point in the code as determined by the
	 *  use of the command coWeave(), further advice is not longer permitted. Similarly, the matching/execution of a pointcut is
	 *  not permitted prior to the command coWeave(). Note - that this not disallow dynamic (aspectual object) advices or dynamic
	 *  pointcuts, but rather that a weave phase is expected to occur. If coFinalise() is call without the weave occurring, an
	 *  error will occur. This mode is useful for testing/debugging.
	 */
	#define coStaticWeave 0x0001

	/** Enforce that only static advice will be used.
	 */
	#define coStaticAdvice 0x0002

	/** Enforce that only static pointcut will be used. In this mode the pointcut stack is not maintained, as its purpose is to
	 *  provide the pointer of the current pointcut in pointcut_ptr(). The positive repercussion of this is that the push & pop
	 *  is not injected into the pointcut weaved code.
	 */
	#define coStaticPointcut 0x0004

#endif /* __Compass_mode_h__ */
