package npc;

public class InvalidGameParameterException extends GameException {
    private final String parameterName;
    private final Object invalidValue;

    public InvalidGameParameterException(String message, String parameterName, Object invalidValue) {
        super(message + " (parameter: " + parameterName + ", value: " + invalidValue + ")");
        this.parameterName = parameterName;
        this.invalidValue = invalidValue;
    }

    public String getParameterName() {
        return parameterName;
    }

    public Object getInvalidValue() {
        return invalidValue;
    }
}
