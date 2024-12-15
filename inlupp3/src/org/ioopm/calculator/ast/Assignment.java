package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.IllegalAssignmentException;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Assignment extends Binary {


    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
    }


    public String getName() {
        return "=";
    }


    public boolean equals(Object other) {
        if(other instanceof Assignment) {
            Assignment tmp = (Assignment) other;
            if(this.getLhs().getValue() == tmp.getLhs().getValue() &&
                    this.getRhs().getValue() == tmp.getRhs().getValue()) {
                return true;
            }
        }
        return false;
    }


    /// When evaluation of the left-hand side is completed, the variable and the evaluation will be stored
    /// within the 'Environment' for future use.
    /// Assignment can't be made to constants
    @Override
    public SymbolicExpression eval(Environment environment) throws IllegalAssignmentException {
        SymbolicExpression lhsEval = this.getLhs().eval(environment);
        SymbolicExpression rhsEval = this.getRhs();

        if(rhsEval.isConstant()) {  // Throw an error if the rhs is a constant
            throw new IllegalAssignmentException();
        }
        if (rhsEval.isVariable()) {  // If fully evaluated - store variable as KEY and evaluation as VALUE in HT
            environment.put((Variable) rhsEval, lhsEval);
            return lhsEval;
        }
        else {
            return new Assignment(lhsEval, rhsEval);
        }
    }


    @Override
    public String toString() {
        return super.toString();
    }
}