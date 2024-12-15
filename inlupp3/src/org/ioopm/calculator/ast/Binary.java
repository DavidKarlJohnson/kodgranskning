package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public abstract class Binary extends SymbolicExpression {
    private SymbolicExpression lhs = null;
    private SymbolicExpression rhs = null;


    public Binary(SymbolicExpression lhs, SymbolicExpression rhs) {
        this.lhs = lhs;
        this.rhs = rhs;
    }


    public SymbolicExpression getLhs() {
        return this.lhs;
    }


    public SymbolicExpression getRhs() {
        return this.rhs;
    }


    public abstract SymbolicExpression eval(Environment environment) throws IllegalAssignmentException;


    ///  Concatenates parentheses around expressions where priority is important
    @Override
    public String toString() {
        StringBuilder sb = new StringBuilder();

        // Left-hand-side
        if(this.lhs instanceof Assignment) {
            sb.append("(").append(this.lhs.toString()).append(") ");
        }
        else if(this.lhs.getPriority() < this.getPriority() && !(this.lhs instanceof Constant)) {
            sb.append("(").append(this.lhs.toString()).append(") ");
        }
        else {
            sb.append(this.lhs.toString()).append(" ");
        }

        sb.append(this.getName()).append(" ");

        // Right-hand-side
        if(this.rhs instanceof Assignment) {
            sb.append("(").append(this.rhs.toString()).append(") ");
        }
        else if(this.rhs.getPriority() < this.getPriority() && !(this.rhs instanceof Constant)) {
            sb.append("(").append(this.rhs.toString()).append(")");
        }
        else if(this.rhs.getPriority() == this.getPriority() && this.lhs.getPriority() == this.getPriority() && !(this.rhs instanceof Constant) && this.getPriority() > 50) {
            sb.append("(").append(this.rhs.toString()).append(")");
        }
        else {
            sb.append(this.rhs.toString());
        }

        return sb.toString();
    }
}
