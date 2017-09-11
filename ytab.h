// Adapted From Clinton Jeffery's File: http://www2.cs.uidaho.edu/~jeffery/courses/445/ytab.h

FILE *yyin; // added this 
struct token *YYTOKEN; 
char* curr_filename; // added this
char* prev_filename;
char* file_name; 
char* temp_name; 
int include; 

/* Size of default input buffer. */
/* Retrieved from lex.yy.c file */
#ifndef YY_BUF_SIZE
#ifdef __ia64__
/* On IA-64, the buffer size is 16k, not 8k.
 * Moreover, YY_BUF_SIZE is 2*YY_READ_BUF_SIZE in the general case.
 * Ditto for the __ia64__ case accordingly.
 */
#define YY_BUF_SIZE 32768
#else
#define YY_BUF_SIZE 16384
#endif /* __ia64__ */
#endif

// added for 120++
#define ARROWSTAR 342
#define THIS 343
#define TRUE 344
#define PUBLIC 345
#define PROTECTED 346
#define PRIVATE 347
#define NEW 348
#define DELETE 349
#define BOOL 350
#define CLASS 351
#define SCON 352
#define ELIPSIS 353
#define COLONCOLON 354
#define FALSE 355
#define CLASS_NAME 356
#define ADDEQ 357
#define SUBEQ 358
#define MULEQ 359
#define DIVEQ 360
#define MODEQ 361
#define XOREQ 362
#define ANDEQ 363
#define OREQ 364
#define SL 365
#define SR 366
#define SLEQ 367
#define SREQ 368
#define NOTEQ 369
#define LTEQ 370
#define GTEQ 371
#define PLUSPLUS 372
#define MINUSMINUS 373  
#define DOTSTAR 374
#define ELLIPSIS 375
#define ASM 376
#define ENUM_NAME 377
#define EXPLICIT 378
#define EXPORT 379
#define INLINE 380
#define MUTABLE 381
#define NAMESPACE 382
#define DYNAMIC_CAST 383
#define REINTERPRET_CAST 384
#define STATIC_CAST 385
#define WCHAR_T 386
#define USING 387
#define THROW 388
#define FRIEND 389
#define TEMPLATE 390
#define CONST_CAST 391
#define OPERATOR 392
#define TRY 393
#define TYPEID 394
#define TYPENAME 395
#define VIRTUAL 396


#define AUTO 258
#define BREAK 259
#define CASE 260
#define CHAR 261
#define CONST 262
#define CONTINUE 263
#define DEFAULT 264
#define DO 265
#define DOUBLE 266
#define ELSE 267
#define ENUM 268
#define EXTERN 269
#define FLOAT 270
#define FOR 271
#define GOTO 272
#define IF 273
#define INT 274
#define LONG 275
#define REGISTER 276
#define RETURN 277
#define SHORT 278
#define SIGNED 279
#define SIZEOF 280
#define STATIC 281
#define STRUCT 282
#define SWITCH 283
#define TYPEDEF 284
#define UNION 285
#define UNSIGNED 286
#define VOID 287
#define VOLATILE 288
#define WHILE 289
#define INTEGER 290 // ICON
#define CHARACTER 291 // CCON
#define FLOATING 292 // FLOATING
#define STRING 293
#define SRASN 294
#define PLASN 295
#define MIASN 296
#define MUASN 297
#define DIASN 298
#define MOASN 299
#define ANASN 300
#define ERASN 301
#define ORASN 302
#define SHR 303
#define SHL 304
#define INCOP 305
#define DECOP 306
#define ARROW 307
#define ANDAND 308
#define OROR 309
#define LE 310
#define SLASN 311
#define GE 312
#define EQ 313
#define NE 314
#define SM 315
#define LC 316
#define RC 317
#define CM 318
#define COLON 319
#define ASN 320
#define LP 321
#define RP 322
#define LB 323
#define RB 324
#define DOT 325
#define AND 326
#define BANG 327
#define NOT 328
#define MINUS 329
#define PLUS 330
#define MUL 331
#define DIV 332
#define MOD 333
#define LT 334
#define GT 335
#define ER 336
#define OR 337
#define QUEST 338
#define IDENTIFIER 339
#define TYPEDEF_NAME 340
#define BAD_TOKEN 341