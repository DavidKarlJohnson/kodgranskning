package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.SymbolicExpression;
import java.util.Objects;

public class Variable extends Atom {
    private String identifier;


    public Variable(String identifier) {
        this.identifier = identifier;
    }


    @Override
    public boolean isVariable() {
        return true;
    }


    /// The hashing into the 'Environment' will be done with the variables identifier instead of the object
    /// Example: the variable x will be hashed with the string "x" and NOT with the object itself
    @Override
    public int hashCode() {
        return this.identifier.hashCode();
    }


    public boolean equals(Object other) {
        if(other instanceof Variable) {
            if(Objects.equals(this.identifier, ((Variable) other).identifier)) {
                return true;
            }
        }
        return false;
    }


    /// If the variable already has a value assigned to it. Fetch it from the 'Environment' instead of creating a new
    @Override
    public SymbolicExpression eval(Environment environment) {
        if(environment.containsKey(this)) {
            return environment.get(this);
        }
        return new Variable(this.identifier);
    }


    @Override
    public String toString() {
        return String.valueOf(this.identifier);
    }
}


