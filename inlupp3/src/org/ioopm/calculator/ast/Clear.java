package org.ioopm.calculator.ast;

import org.ioopm.calculator.Environment;
import org.ioopm.calculator.parser.SymbolicExpression;

public class Clear extends Command {

    private static final Clear theInstance = new Clear();


    private Clear() {}


    public static Clear instance() {
        return theInstance;
    }


    @Override
    public boolean isCommand() {
        return true;
    }


    @Override
    public SymbolicExpression eval(Environment environment) {
        return null;
    }
}


