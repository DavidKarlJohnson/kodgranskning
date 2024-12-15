package org.ioopm.calculator.parser;
import org.ioopm.calculator.Environment;
import org.ioopm.calculator.ast.*;

public abstract class SymbolicExpression {
    private String[] subExpressions;
    protected int priority = 100;


    public SymbolicExpression(Object... subExpressions) {
        this.subExpressions = new String[subExpressions.length];
        for (int i = 0; i < subExpressions.length; ++i) {
            this.subExpressions[i] = subExpressions[i].toString();
        }
    }


    public boolean isConstant() {
        return false;
    }


    public boolean isVariable() {
        return false;
    }


    public boolean isCommand() {
        return false;
    }


    public int getPriority () {
        return priority;
    }


    public String getName() {
        throw new RuntimeException("getName() called on expression with no operator");
    }


    public double getValue() {
        if (this instanceof Constant) {
            return this.getValue();
        } else {
            throw new IllegalArgumentException("Object is not an instance of Constant.");
        }
    }


    public abstract SymbolicExpression eval(Environment environment) throws IllegalAssignmentException;
}