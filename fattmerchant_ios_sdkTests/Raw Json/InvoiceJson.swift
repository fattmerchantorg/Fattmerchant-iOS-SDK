//
//  InvoiceJson.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/22/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

let invoiceJson =
"""
{
    "id": "70a5c89f-8123-43e2-88e9-7cba72536fcd",
    "merchant_id": "a61d78cc-cde9-44ac-8a18-30c39be05879",
    "user_id": "30c6eeb6-64b6-47f6-bcf6-787a9c58798b",
    "customer_id": null,
    "total": 0.02,
    "meta": {
        "subtotal": "0.02"
    },
    "status": "DRAFT",
    "is_merchant_present": true,
    "sent_at": null,
    "viewed_at": null,
    "paid_at": null,
    "schedule_id": null,
    "reminder_id": null,
    "payment_method_id": null,
    "url": "https://qa.fattpay.com/#/bill/",
    "is_webpayment": false,
    "deleted_at": null,
    "created_at": "2020-01-22 13:03:13",
    "updated_at": "2020-01-22 13:03:13",
    "due_at": null,
    "is_partial_payment_enabled": true,
    "invoice_date_at": "2020-01-22 13:03:13",
    "payment_attempt_failed": false,
    "payment_attempt_message": "",
    "balance_due": 0.02,
    "total_paid": 0,
    "payment_meta": [],
    "customer": null,
    "user": {
        "id": "30c6eeb6-64b6-47f6-bcf6-787a9c58798b",
        "system_admin": true,
        "name": "Tulio",
        "email": "tulio@fattmerchant.com",
        "email_verification_sent_at": null,
        "email_verified_at": "2018-01-15 20:06:30",
        "is_api_key": false,
        "acknowledgments": {
            "tutorial": true,
            "godviewStaxSearchTooltip": true,
            "godviewWelcome": true
        },
        "created_at": "2017-11-20 18:08:08",
        "updated_at": "2019-01-03 08:43:23",
        "deleted_at": null,
        "brand": "fattmerchant",
        "gravatar": "//www.gravatar.com/avatar/f2e9750e88901361d847c738a7211efe",
        "team_admin": null,
        "team_enabled": null,
        "team_role": null,
        "merchant_options": [],
        "is_default": false
    },
    "files": [],
    "child_transactions": [],
    "reminder": null
}
"""
