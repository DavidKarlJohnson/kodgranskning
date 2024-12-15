package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.SymbolicExpression;


public class Vars extends Command {
    private static final Vars theInstance = new Vars();


    public static Vars instance() {
        return theInstance;
    }
    @Override
    public SymbolicExpression eval(Environment environment) {
        return null;
    }


    @Override
    public boolean isCommand() {
        return true;
    }
}
