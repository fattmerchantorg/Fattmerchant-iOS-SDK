//
//  MobileReaderPickerController.swift
//  Fattmerchant-iOS-SDK-Sample
//
//  Created by Tulio Troncoso on 1/23/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

import Foundation
import UIKit

/// Controls a UIPickerView
class StringPickerController<T: CustomStringConvertible>: UIViewController, UIPickerViewDelegate,
                                                          UIPickerViewDataSource {

  var values: [T] = []
  var handler: (T?) -> Void

  init(values: [T], handler: @escaping (T?) -> Void) {
    self.values = values
    self.handler = handler
    super.init(nibName: nil, bundle: nil)
  }

  required init?(coder: NSCoder) {
    fatalError("init(coder:) has not been implemented")
  }

  override func loadView() {
//    let view = UIView()
    let picker = UIPickerView()
//    view.addSubview(picker)
    picker.backgroundColor = UIColor.white
//
//    picker.leadingAnchor.constraint(equalTo: view.leadingAnchor).isActive = true
//    picker.trailingAnchor.constraint(equalTo: view.trailingAnchor).isActive = true
//    picker.bottomAnchor.constraint(equalTo: view.safeAreaLayoutGuide.bottomAnchor).isActive = true
//    picker.heightAnchor.constraint(equalToConstant: 60.0)

    self.view = picker
  }

  override func viewDidLoad() {
    (view as? UIPickerView)?.delegate = self
    (view as? UIPickerView)?.dataSource = self
  }

  func numberOfComponents(in pickerView: UIPickerView) -> Int {
    return 1
  }

  func pickerView(_ pickerView: UIPickerView, numberOfRowsInComponent component: Int) -> Int {
    return values.count
  }

  func pickerView(_ pickerView: UIPickerView, titleForRow row: Int, forComponent component: Int) -> String? {
    if values.count > row {
      return values[row].description
    } else {
      return ""
    }
  }

  func pickerView(_ pickerView: UIPickerView, didSelectRow row: Int, inComponent component: Int) {
    if values.count > row {
      handler(values[row])
    } else {
      handler(nil)
    }
  }
}
