#include <Compass.h>

#include <stdio.h>

/* Test the case where an advice has an aspect in more than one pointcut ... ensure AST is ok */

typedef void (TestPrototype) (void);
typedef struct {
	int foo;
} TestClass;

#if 0
void testXX(void)
{
	// just want to see what AST of this looks like...
	void *foo =
	    ((coAspectFunc *) (coAspects_s->jps->jp[7]->_funcMetas->data[13]))->
	    fromAspect;
}
#endif

void test1(void)
{
	TestClass *self = aspect_ptr();

	self->foo = 10;
	/* __func__ should still result in "test1", not "main" once weaved */
	printf("In function \"%s\"\n", __func__);
}

void test2(void)
{
	TestClass *self = aspect_ptr();
	int i;

	for (i = 0; i < 3; i++) {
		/* There's no need for "result" to be mangled/made-unique */
		/* "result" should be 10 * [0..2] = [0..20] */
		int result = self->foo * i;

		/* __func__ should still result in "test2", not "main" once weaved */
		printf("In function \"%s\", result = %d\n", __func__, result);
	}
}

void test3(void)
{
	TestClass *self = aspect_ptr();

	self->foo = 100;
	/* __func__ should still result in "test3", not "main" once weaved */
	printf("In function \"%s\"\n", __func__);
}

void body1();

void body2();

int main(int argc, char *argv[])
{
	TestClass *ao = malloc(sizeof(TestClass));

	coInit(&argc, &argv, coStaticWeave | coStaticPointcut);

	advice_aspect(body1, TestPrototype, test1, coAdviceAppend, ao);
	advice_aspect(body1, TestPrototype, test2, coAdviceAppend, ao);

	advice_aspect(body2, TestPrototype, test3, coAdviceAppend, ao);
	advice_aspect(body2, TestPrototype, test2, coAdviceAppend, ao);

	coWeave();

	body1();
	body2();

	coFinalise();
	return 0;
}

void body1()
{
	pointcut(TestPrototype);
}

void body2()
{
	pointcut(TestPrototype);
}
