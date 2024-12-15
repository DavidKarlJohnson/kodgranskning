package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Addition extends Binary {


    public Addition(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
        this.priority = 50;
    }


    public String getName() {
        return "+";
    }


    public boolean equals(Object other) {
        if(other instanceof Addition) {
            Addition tmp = (Addition) other;
            if(this.getLhs().getValue() == tmp.getLhs().getValue() ||
               this.getRhs().getValue() == tmp.getRhs().getValue()) {
                return true;
            }
        }
        return false;
    }


    /// If both the lgs and the rhs get fully evaluated, then a final addition will be made
    /// and the calculated CONSTANT will be returned.
    /// If not: A new addition expression will be returned. For example: x+y, are not fully evaluated
    /// constants, they're variables
    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression lhsEval = this.getLhs().eval(environment);
        SymbolicExpression rhsEval = this.getRhs().eval(environment);

        if (lhsEval.isConstant() && rhsEval.isConstant()) {  // Fully evaluated, return CONSTANT
            return new Constant(lhsEval.getValue() + rhsEval.getValue());
        }
        else {  // Return addition EXPRESSION
            return new Addition(lhsEval, rhsEval);
        }
    }


    @Override
    public String toString() {
        return super.toString();
    }
}

