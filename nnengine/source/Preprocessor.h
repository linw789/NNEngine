#ifndef NNE_PREPROCESSOR_H
#define NNE_PREPROCESSOR_H

/*
	C Preprocessor library
	reference: http://www.altdevblogaday.com/2011/07/12/abusing-the-c-preprocessor/
*/


/*
	#define NNE_JOIN(X, Y) X##Y
	#define TEST1 CANCATE 
	#define TEST2 NATION 

	NNE_JOIN(TEST1, TEST2) => TEST1TEST2 (not CANCATENATION)
	because the cancatenation happens before preprocessor directives being evaluated
	so we need to add a layer of indirection, evaluate X and Y first then join them
*/
#define NNE_JOIN_(X, Y) X##Y
#define NNE_JOIN(X, Y) NNE_JOIN_(X, Y)

// binary logic
#define NNE_TO_BOOL_0 0
#define NNE_TO_BOOL_1 1
#define NNE_TO_BOOL_2 1
#define NNE_TO_BOOL_3 1
#define NNE_TO_BOOL_4 1
#define NNE_TO_BOOL_5 1
#define NNE_TO_BOOL_6 1
#define NNE_TO_BOOL_7 1
#define NNE_TO_BOOL_8 1
#define NNE_TO_BOOL_9 1

#define NNE_PP_TO_BOOL(X) NNE_JOIN(NNE_TO_BOOL_, X)

// bitwise not
#define NNE_OP_NOT_0 1
#define NNE_OP_NOT_1 0
#define NNE_PP_OP_NOT(X) NNE_JOIN(NNE_OP_NOT_, NNE_PP_TO_BOOL(X))

// bitwise or
#define NNE_OP_OR_00 0
#define NNE_OP_OR_01 1
#define NNE_OP_OR_10 1
#define NNE_OP_OR_11 1
#define NNE_PP_OP_OR(X, Y) NNE_JOIN(NNE_OP_OR_, NNE_JOIN(NNE_PP_TO_BOOL(X), NNE_PP_TO_BOOL(Y)))

// bitwise and
#define NNE_OP_AND_00 0
#define NNE_OP_AND_01 0
#define NNE_OP_AND_10 0
#define NNE_OP_AND_11 1
#define NNE_PP_OP_AND(X, Y) NNE_JOIN(NNE_OP_AND_, NNE_JOIN(NNE_PP_TO_BOOL(X), NNE_PP_TO_BOOL(Y)))

// contionals
#define NNE_OP_IF_0(X)
#define NNE_OP_IF_1(X) X
#define NNE_PP_OP_IF(COND, X) NNE_JOIN(NNE_OP_IF_, NNE_PP_TO_BOOL(COND))(X)

#define NNE_OP_IF_ELSE_0(X, Y) Y
#define NNE_OP_IF_ELSE_1(X, Y) X
#define NNE_PP_OP_IF_ELSE(COND, X, Y) NNE_JOIN(NNE_OP_IF_ELSE_, NNE_PP_TO_BOOL(COND))(X, Y)

#define NNE_PP_EMPTY() 
#define NNE_PP_COMMA() ,
#define NNE_PP_COMMA_IF(COND) NNE_PP_OP_IF_ELSE(COND, NNE_PP_COMMA, NNE_PP_EMPTY)()

// listing sequential symbols, separated by comma
#define NNE_SEQ_0(X) 
#define NNE_SEQ_1(X) X##0
#define NNE_SEQ_2(X) NNE_SEQ_1(X), X##1
#define NNE_SEQ_3(X) NNE_SEQ_2(X), X##2
#define NNE_SEQ_4(X) NNE_SEQ_3(X), X##3
#define NNE_SEQ_5(X) NNE_SEQ_4(X), X##4
#define NNE_SEQ_6(X) NNE_SEQ_5(X), X##5
#define NNE_SEQ_7(X) NNE_SEQ_6(X), X##6
#define NNE_PP_SEQUENCE(N, X) NNE_JOIN(NNE_SEQ_, N)(X)

#define NNE_SEQ_TWO_SYMBOL_0(X, Y)
#define NNE_SEQ_TWO_SYMBOL_1(X, Y) X##0 Y##0
#define NNE_SEQ_TWO_SYMBOL_2(X, Y) NNE_SEQ_TWO_SYMBOL_1(X, Y), X##1 Y##1
#define NNE_SEQ_TWO_SYMBOL_3(X, Y) NNE_SEQ_TWO_SYMBOL_2(X, Y), X##2 Y##2
#define NNE_SEQ_TWO_SYMBOL_4(X, Y) NNE_SEQ_TWO_SYMBOL_3(X, Y), X##3 Y##3
#define NNE_SEQ_TWO_SYMBOL_5(X, Y) NNE_SEQ_TWO_SYMBOL_4(X, Y), X##4 Y##4
#define NNE_SEQ_TWO_SYMBOL_6(X, Y) NNE_SEQ_TWO_SYMBOL_5(X, Y), X##5 Y##5
#define NNE_SEQ_TWO_SYMBOL_7(X, Y) NNE_SEQ_TWO_SYMBOL_6(X, Y), X##6 Y##6
#define NNE_PP_SEQUENCE_2SYMBOL(N, X, Y) NNE_JOIN(NNE_SEQ_TWO_SYMBOL_, N)(X, Y)

// repeat the symbol, separated by comma
#define NNE_REPEAT_0(X) 
#define NNE_REPEAT_1(X) X
#define NNE_REPEAT_2(X) NNE_REPEAT_1(X), X
#define NNE_REPEAT_3(X) NNE_REPEAT_2(X), X
#define NNE_REPEAT_4(X) NNE_REPEAT_3(X), X
#define NNE_REPEAT_5(X) NNE_REPEAT_4(X), X
#define NNE_REPEAT_6(X) NNE_REPEAT_5(X), X
#define NNE_REPEAT_7(X) NNE_REPEAT_6(X), X
#define NNE_PP_REPEAT(N, X) NNE_JOIN(NNE_REPEAT_, N)(X)

#endif // NNE_PREPROCESSOR_H