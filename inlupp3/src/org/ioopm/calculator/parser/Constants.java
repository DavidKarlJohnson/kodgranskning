package org.ioopm.calculator.parser;
import java.util.HashMap;

/// NamedConstants will be sored in the hashmap within this class.
/// Whenever a named constant is called from the console, its value should be fetched from this hashmap
public class Constants {
    public static final HashMap<String, Double> namedConstants = new HashMap<>();

    static {
        Constants.namedConstants.put("pi", Math.PI);
        Constants.namedConstants.put("e",  Math.E);
        Constants.namedConstants.put("answer",  42.0);
        Constants.namedConstants.put("l",  6.022140857 * Math.pow(10, 23));
    }
}
