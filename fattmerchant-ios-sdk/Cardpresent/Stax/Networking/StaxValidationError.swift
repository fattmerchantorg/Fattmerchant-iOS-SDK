/// Error type for validation failures
public enum StaxValidationError: Error {
  case missingRequiredFields(String)
}
