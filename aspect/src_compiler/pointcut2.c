#include <Compass.h>

#include <stdio.h>

typedef void (TestPrototype) (void);
typedef struct {
	int foo;
} TestClass;

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

void body1();

int main(int argc, char *argv[])
{
	TestClass *ao = malloc(sizeof(TestClass));

	coInit(&argc, &argv, coStaticWeave | coStaticPointcut);

	advice_aspect(body1, TestPrototype, test1, coAdviceAppend, ao);
	advice_aspect(body1, TestPrototype, test2, coAdviceAppend, ao);

	coWeave();

	body1();

	coFinalise();
	return 0;
}

void body1()
{
	pointcut(TestPrototype);
}
