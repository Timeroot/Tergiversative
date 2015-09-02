/*
 * Clause.cpp
 *
 *  Created on: Aug 27, 2015
 *      Author: Timeroot
 */

#include "Clause.h"

Clause::Clause(const Clause& other) : n(other.n), a(other.a), b(other.b), c(other.c) {}

Clause::Clause() : n(0) {}

Clause::Clause(int a_) : n(1), a(a_) {}

Clause::Clause(int a_, int b_) : n(2), a(a_), b(b_) {}

Clause::Clause(int a_, int b_, int c_) : n(3), a(a_), b(b_), c(c_) {}

bool Clause::operator==(const Clause& other) const{
	if(n!=other.n)
		return false;
	switch(n){
	case 3: if(c!=other.c) return false;
	/* no break */
	case 2: if(b!=other.b) return false;
	/* no break */
	case 1: if(a!=other.a) return false;
	}
	return true;
}

bool Clause::mentions(int var) const{
	switch(n){
	case 3:
		if(VAR(c)==var)
			return true;
	/* no break */
	case 2:
		if(VAR(b)==var)
			return true;
	/* no break */
	case 1:
		if(VAR(a)==var)
			return true;
	}
	return false;
}

bool Clause::assign(VarAssignment newAss, Clause& dest) const{
	switch(n){
		case 3:
			if(VAR(a)==VAR(newAss.val)){
				if(a==newAss.val){
					return true;
				} else {
					dest = Clause(b,c);
					return false;
				}
			}
			if(VAR(b)==VAR(newAss.val)){
				if(b==newAss.val){
					return true;
				} else {
					dest = Clause(a,c);
					return false;
				}
			}
			if(VAR(c)==VAR(newAss.val)){
				if(c==newAss.val){
					return true;
				} else {
					dest = Clause(a,b);
					return false;
				}
			}
			break;

		case 2:
			if(VAR(a)==VAR(newAss.val)){
				if(a==newAss.val){
					return true;
				} else {
					dest = Clause(b);
					return false;
				}
			}
			if(VAR(b)==VAR(newAss.val)){
				if(b==newAss.val){
					return true;
				} else {
					dest = Clause(a);
					return false;
				}
			}
			break;

		case 1:
			if(VAR(a)==VAR(newAss.val)){
				if(a==newAss.val){
					return true;
				} else {
					dest = Clause();
					return false;
				}
			}
			break;
	}
	throw "Assigned irrelevant variable";
}
