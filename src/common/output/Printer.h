/* 
 * File:   Printer.h
 * Author: opiske
 *
 * Created on May 7, 2015, 9:48 AM
 */

#ifndef PRINTER_H
#define	PRINTER_H

#include <cstdio>

class Printer {
public:
    /**
     * Enumerates the numbering base for the output
     */
    enum Base {
        HEXADECIMAL, 	/*!< Hexadecimal base */
        DECIMAL,	/*!< Decimal base */
        OCTAL		/*!< Octal base */
    };
                
    
    Printer();
    Printer(FILE *stream);
    Printer(const Printer& orig);
    virtual ~Printer();
    
    /**
     * Prints a signed integer
     * @param val Value
     */
    void print(int val);

    /**
     * Prints an unsigned integer
     * @param val Value
     */
    void print(unsigned int val);


    /**
    * Prints a signed long integer
//    * @param val Value
    */
    void print(long int val);

    /**
    * Prints an unsigned long integer
    * @param val Value
    */
    void print(unsigned long int val);

    /**
    * Prints a double
    * @param d Value
    */
    void print(double d);

    /**
    * Prints a char
    * @param c Char
    */
    void print(char c);

    /**
    * Prints the address of a pointer
    * @param ptr The address of the pointer
    */
    void print(const void *ptr);


    /**
     * Prints a char * string
     * @param str A pointer to a null terminated string
     *  This prevents the compiler from  casting a regular, temporary, string to
     * a const void pointer, which would cause the print(const void *) method 
      * to be called
     */
    void print(const char *str);
private:
    Base base;
    FILE *stream;

};

#endif	/* PRINTER_H */

