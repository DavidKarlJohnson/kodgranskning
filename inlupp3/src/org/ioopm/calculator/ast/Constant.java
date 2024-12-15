package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Constant extends Atom {
    private double value;


    public Constant(double value) {
        this.value = value;
        this.priority = 70;
    }


    @Override
    public boolean isConstant() {
        return true;
    }


    public double getValue() {
        return this.value;
    }


    public boolean equals(Object other) {
        if(other instanceof Constant) {
            if(this.value == ((Constant) other).value) {
                return true;
            }
        }
        return false;
    }


    @Override
    public SymbolicExpression eval(Environment environment) {
        return new Constant(this.value);
    }


    @Override
    public String toString() {
        return String.valueOf(this.value);
    }
}
