package org.ioopm.calculator.ast;

import org.ioopm.calculator.parser.SymbolicExpression;

public abstract class Unary extends SymbolicExpression {
    protected SymbolicExpression value;


    public Unary(SymbolicExpression value) {
        this.value = value;
    }


    @Override
    public String toString() {
        return this.getName() + "(" + this.value.toString() + ")";
    }
}