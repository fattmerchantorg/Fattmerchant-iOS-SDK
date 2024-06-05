protocol ServiceLocator {
  static func register<T>(_ type: T.Type, service: Any)
  static func resolve<T>(_ type: T.Type) -> T?
}

class Services: ServiceLocator {
  private static var services: [String: Any] = [:]
    
  public static func register<T>(_ type: T.Type, service: Any) {
    let key = String(describing: type)
    services[key] = service
  }
    
  public static func resolve<T>(_ type: T.Type) -> T? {
    let key = String(describing: type)
    return services[key] as? T
  }
}
