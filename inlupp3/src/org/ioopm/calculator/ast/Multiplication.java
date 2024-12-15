package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Multiplication extends Binary {


    public Multiplication(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
        this.priority = 70;
    }


    public String getName() {
        return "*";
    }


    public boolean equals(Object other) {
        if(other instanceof Multiplication) {
            Multiplication tmp = (Multiplication) other;
            if(this.getLhs().getValue() == tmp.getLhs().getValue() ||
               this.getRhs().getValue() == tmp.getRhs().getValue()) {
                return true;
            }
        }
        return false;
    }


    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression lhsEval = this.getLhs().eval(environment);
        SymbolicExpression rhsEval = this.getRhs().eval(environment);

        if (lhsEval.isConstant() && rhsEval.isConstant()) {
            return new Constant(lhsEval.getValue() * rhsEval.getValue());
        }
        else {
            return new Multiplication(lhsEval, rhsEval);
        }
    }


    @Override
    public String toString() {
        return super.toString();
    }
}
