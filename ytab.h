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

// add define for string constant? 
#define FOLLOWPTR 246
#define THIS 247
#define TRUE 248
#define PUBLIC 249
#define PROTECTED 250
#define PRIVATE 251
#define NEW 252
#define DELETE 253
#define BOOL 254
#define CLASS 255
#define SCON 256
#define ELIPSIS 257
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
#define ICON 290
#define CCON 291
#define FCON 292
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
#define FOLLOW 307
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