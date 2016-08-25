#include <Compass.h>

#include <stdio.h>

typedef void (TestPrototype) (int i);

void test1(int i)
{
	printf("In function \"%s\", i = %d\n", __func__, i);
}

void test2(int i)
{
	printf("In function \"%s\", i = %d\n", __func__, i);
}

int main(int argc, char *argv[])
{
	coInit(&argc, &argv, coStaticWeave | coStaticAdvice | coStaticPointcut);

	advice(main, TestPrototype, test1, coAdviceAppend);
	advice(main, TestPrototype, test2, coAdviceAppend);

	coWeave();

	pointcut(TestPrototype, 23);

	coFinalise();
	return 0;
}
