/******************************************************************************/
/**
@file		slhandler.c
@author		Kris Wallperington
@brief		Unit tests for skiplist handler interface.
*/
/******************************************************************************/

#include "sltests.h"

/**
@brief 		Helper function that constructs a sample dictionary based on the
			given parameters.

@param 		dictionary
				Dictionary to initialize
@param 		handler
				Handler to bind
@param 		record
				Secondary information within dictionary
@param 		key_type
				Type of key used
@param 		size
				Size of dictionary
 */
void
create_test_collection(
	dictionary_t			*dictionary,
	dictionary_handler_t 	*handler,
	record_info_t 			*record,
	key_type_t 				key_type,
	int						size,
	int 					num_elements
)
{
	sldict_init(handler);

	dictionary_create(
			handler,
			dictionary,
			key_type,
			record->key_size,
			record->value_size,
			size
	);

	/* Populate dictionary */
	int 	half_elements 	= num_elements / 2;
	char* 	value 			= "DATA";
	int 	i,j;
	/* First insert one of each element, up to half... */
	for(i = 0; i < half_elements; i++)
	{
		dictionary_insert(dictionary, (ion_key_t) &i, (ion_value_t) value);
	}
	/* Continue inserting, this time with an increasing amount of duplicates */
	for(; i < num_elements; i++)
	{
		for(j = half_elements; j < i; j++)
		{
			dictionary_insert(dictionary, (ion_key_t) &i, (ion_value_t) value);
		}
	}
}

/**
@brief 		Helper function to create a collection using standard condition
			variables.

@param 		dictionary
				Dictionary to initialize
@param 		handler
				Handler to bind
 */
void
create_test_collection_std_conditions(
	dictionary_t 			*dictionary,
	dictionary_handler_t 	*handler
)
{
	/* This means keysize 4 and valuesize 10 */
	record_info_t 			record 			= {4, 10};
	key_type_t 				key_type 		= key_type_numeric_signed;
	int 					size 			= 7;
	int 					num_elements 	= 100;

	create_test_collection(
			dictionary,
			handler,
			&record,
			key_type,
			size,
			num_elements
	);
}

/**
@brief 		Tests the creation of a handler and verifies all function pointers
			have been correctly bound.

@param 		tc
				CuTest dependency
 */
void
test_collection_handler_binding(
	CuTest 		*tc
)
{
	PRINT_HEADER();
	dictionary_handler_t 	handler;
	sldict_init(&handler);

	CuAssertTrue(tc, handler.insert				== &sldict_insert);
	CuAssertTrue(tc, handler.create_dictionary	== &sldict_create_dictionary);
	CuAssertTrue(tc, handler.update				== &sldict_update);
	CuAssertTrue(tc, handler.delete				== &sldict_delete);
	CuAssertTrue(tc, handler.delete_dictionary	== &sldict_delete_dictionary);
}

/**
@brief 		Tests the use of an equality
 */

/**
@brief 		Tests the creation of a collection and verifies all properties
			have been correctly initialized.

@param 		tc
				CuTest dependency
 */
void
test_collection_creation(
	CuTest 		*tc
)
{
	PRINT_HEADER();
	dictionary_t 			dict;
	dictionary_handler_t 	handler;
	record_info_t 			record 			= {4,10};
	key_type_t 				key_type 		= key_type_numeric_signed;
	int 					size 			= 50;
	int 					num_elements 	= 25;

	create_test_collection(
			&dict,
			&handler,
			&record,
			key_type,
			size,
			num_elements
	);

	skiplist_t		*skiplist = (skiplist_t*) dict.instance;

	CuAssertTrue(tc, dict.instance->key_type 			== key_type_numeric_signed);
	CuAssertTrue(tc, dict.instance->compare 			== dictionary_compare_signed_value);
	CuAssertTrue(tc, dict.instance->record.key_size 	== 4);
	CuAssertTrue(tc, dict.instance->record.value_size == 10);
	CuAssertTrue(tc, skiplist 							!= NULL);
	CuAssertTrue(tc, skiplist->head 					!= NULL);
	CuAssertTrue(tc, skiplist->maxheight 				== 50);
	CuAssertTrue(tc, skiplist->pden 					== 4);
	CuAssertTrue(tc, skiplist->pnum 					== 1);
	CuAssertTrue(tc, &skiplist->super 					!= NULL);

	dictionary_delete_dictionary(&dict);
}

/**
@brief 		Tests an equality cursor on the std conditions skiplist. Assertion
			is that the cursor will return err_ok.

@param 		tc
				CuTest dependency
*/
void
test_slhandler_cursor_equality(
	CuTest 		*tc
)
{
	PRINT_HEADER();
	dictionary_t 			dict;
	dictionary_handler_t 	handler;
	create_test_collection_std_conditions(&dict, &handler);
	dict_cursor_t 			*cursor;
	predicate_t 			predicate;
	predicate.type 									= predicate_equality;
	predicate.statement.equality.equality_value 	= malloc(dict.instance->record.key_size);
	memcpy(predicate.statement.equality.equality_value, (ion_key_t) &(int){33}, dict.instance->record.key_size);

	err_t 	status = dict.handler->find(&dict, &predicate, &cursor);

	CuAssertTrue(tc, err_ok == status);

	cursor->destroy(&cursor);

	CuAssertTrue(tc, NULL == cursor);

	dictionary_delete_dictionary(&dict);
}

/**
@brief 		Tests an equality cursor on the std conditions skiplist. The
			assertion is that the values returned will satisfy the predicate.

@param 		tc
				CuTest dependency
*/
void
test_slhandler_cursor_equality_with_results(
	CuTest 		*tc
)
{
	PRINT_HEADER();
	dictionary_t 			dict;
	dictionary_handler_t 	handler;
	create_test_collection_std_conditions(&dict, &handler);
	dict_cursor_t 			*cursor;
	predicate_t 			predicate;
	predicate.type 									= predicate_equality;
	predicate.statement.equality.equality_value 	= malloc(dict.instance->record.key_size);
	memcpy(predicate.statement.equality.equality_value, (ion_key_t) &(int){56}, dict.instance->record.key_size);

	err_t 	status = dict.handler->find(&dict, &predicate, &cursor);

	CuAssertTrue(tc, err_ok 				== status);
	CuAssertTrue(tc, cs_cursor_initialized 	== cursor->status);

	ion_record_t 	record;
	record.key 					= malloc(dict.instance->record.key_size);
	record.value 				= malloc(dict.instance->record.value_size);

	cursor_status_t c_status = cursor->next(cursor, &record);
	CuAssertTrue(tc, cs_cursor_active == c_status);
	CuAssertTrue(tc, dict.instance->compare(record.key, (ion_key_t) &(int){56}, dict.instance->record.key_size) == 0);
	CuAssertTrue(tc, dict.instance->compare(record.value, (ion_value_t) (char*){"DATA"}, dict.instance->record.value_size) == 0);

	while( cursor->next(cursor, &record) != cs_end_of_results)
	{
		CuAssertTrue(tc, dict.instance->compare(record.key, (ion_key_t) &(int){56}, dict.instance->record.key_size) == 0);
		CuAssertTrue(tc, dict.instance->compare(record.value, (ion_value_t) (char*){"DATA"}, dict.instance->record.value_size) == 0);
	}

	CuAssertTrue(tc, cs_end_of_results 	== cursor->status);

	cursor->destroy(&cursor);
	CuAssertTrue(tc, NULL == cursor);

	dictionary_delete_dictionary(&dict);
}

/**
@brief 		Tests a range cursor on the std conditions skiplist. Assertion
			is that the cursor will return err_ok.

@param 		tc
				CuTest dependency
*/
void
test_slhandler_cursor_range(
	CuTest 		*tc
)
{
	PRINT_HEADER();
	dictionary_t 			dict;
	dictionary_handler_t 	handler;
	create_test_collection_std_conditions(&dict, &handler);
	dict_cursor_t 			*cursor;
	predicate_t 			predicate;
	predicate.type 									= predicate_range;
	predicate.statement.range.leq_value 	= malloc(dict.instance->record.key_size);
	predicate.statement.range.geq_value 	= malloc(dict.instance->record.key_size);
	memcpy(predicate.statement.range.leq_value, (ion_key_t) &(int){15}, dict.instance->record.key_size);
	memcpy(predicate.statement.range.geq_value, (ion_key_t) &(int){60}, dict.instance->record.key_size);

	err_t 	status = dict.handler->find(&dict, &predicate, &cursor);

	CuAssertTrue(tc, err_ok == status);

	cursor->destroy(&cursor);

	CuAssertTrue(tc, NULL == cursor);

	dictionary_delete_dictionary(&dict);
}

/**
@brief 		Tests a range cursor on the std conditions skiplist. The
			assertion is that the values returned will satisfy the predicate.

@param 		tc
				CuTest dependency
*/
void
test_slhandler_cursor_range_with_results(
	CuTest 		*tc
)
{
	PRINT_HEADER();
	dictionary_t 			dict;
	dictionary_handler_t 	handler;
	create_test_collection_std_conditions(&dict, &handler);
	dict_cursor_t 			*cursor;
	predicate_t 			predicate;
	predicate.type 									= predicate_range;
	predicate.statement.range.leq_value 	= malloc(dict.instance->record.key_size);
	predicate.statement.range.geq_value 	= malloc(dict.instance->record.key_size);
	memcpy(predicate.statement.range.leq_value, (ion_key_t) &(int){5}, dict.instance->record.key_size);
	memcpy(predicate.statement.range.geq_value, (ion_key_t) &(int){78}, dict.instance->record.key_size);

	err_t 	status = dict.handler->find(&dict, &predicate, &cursor);

	CuAssertTrue(tc, err_ok 				== status);
	CuAssertTrue(tc, cs_cursor_initialized 	== cursor->status);

	ion_record_t 	record;
	record.key 					= malloc(dict.instance->record.key_size);
	record.value 				= malloc(dict.instance->record.value_size);

	cursor_status_t c_status = cursor->next(cursor, &record);
	CuAssertTrue(tc, cs_cursor_active == c_status);
	CuAssertTrue(tc, dict.instance->compare(record.key, (ion_key_t) &(int){5}, dict.instance->record.key_size) >= 0);
	CuAssertTrue(tc, dict.instance->compare(record.key, (ion_key_t) &(int){78}, dict.instance->record.key_size) <= 0);
	CuAssertTrue(tc, dict.instance->compare(record.value, (ion_value_t) (char*){"DATA"}, dict.instance->record.value_size) == 0);

	while( cursor->next(cursor, &record) != cs_end_of_results)
	{
		CuAssertTrue(tc, dict.instance->compare(record.key, (ion_key_t) &(int){5}, dict.instance->record.key_size) >= 0);
		CuAssertTrue(tc, dict.instance->compare(record.key, (ion_key_t) &(int){78}, dict.instance->record.key_size) <= 0);
		CuAssertTrue(tc, dict.instance->compare(record.value, (ion_value_t) (char*){"DATA"}, dict.instance->record.value_size) == 0);
	}

	CuAssertTrue(tc, cs_end_of_results 	== cursor->status);

	cursor->destroy(&cursor);
	CuAssertTrue(tc, NULL == cursor);

	dictionary_delete_dictionary(&dict);
}

/**
@brief 		Creates the suite to test using CuTest.
@return 	Pointer to a CuTest suite.
 */
CuSuite*
skiplist_handler_getsuite()
{
	CuSuite *suite = CuSuiteNew();

	/* Creation tests */
	SUITE_ADD_TEST(suite, test_collection_handler_binding);
	SUITE_ADD_TEST(suite, test_collection_creation);

	/* Cursor Equality tests */
	SUITE_ADD_TEST(suite, test_slhandler_cursor_equality);
	SUITE_ADD_TEST(suite, test_slhandler_cursor_equality_with_results);

	/* Cursor Range tests */
	SUITE_ADD_TEST(suite, test_slhandler_cursor_range);
	SUITE_ADD_TEST(suite, test_slhandler_cursor_range_with_results);

	return suite;
}

/**
@brief 		Runs all skiplist related tests and outputs the result.
 */
void
runalltests_skiplist_handler()
{
	CuString	*output	= CuStringNew();
	CuSuite		*suite	= skiplist_handler_getsuite();

	CuSuiteRun(suite);
	CuSuiteSummary(suite, output);
	CuSuiteDetails(suite, output);
	printf("----\nSkiplist Handler Tests:\n%s\n", output->buffer);

	CuSuiteDelete(suite);
	CuStringDelete(output);
}