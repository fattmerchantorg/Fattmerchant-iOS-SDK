//
//  MerchantJson.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/14/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

/// Raw json that represents a merchant
let merchantJson =
"""
{
    "id": "a61d78cc-cde9-44ac-8a18-30c39be05879",
    "mid": "redacted",
    "status": "ACTIVE",
    "subdomain": "",
    "company_name": "QBO TEST OAUTH 2",
    "display_name": "QBO TEST OAUTH 2",
    "contact_name": "Daniel Walker",
    "contact_email": "daniel+demo@fattmerchant.com",
    "contact_phone": "2032092403",
    "address_1": "123 Test Avenue",
    "address_2": "",
    "address_city": "Orlando",
    "address_state": "FL",
    "address_zip": "32801",
    "hosted_payments_token": "demobusiness",
    "plan": {
        "id": "fcbcd0b6-ebe5-4ab7-9b6b-f95a6a257acd",
        "merchant_id": "a61d78cc-cde9-44ac-8a18-30c39be05879",
        "user_id": "53a6e82e-008c-40fb-aabf-651567e6d8ce",
        "name": "premium",
        "created_at": "2016-10-07 16:39:52",
        "updated_at": "2016-10-07 16:39:52"
    },
    "options": {
        "tax": null,
        "tax_rates": [
            {
                "id": "a24f-847a",
                "name": "Tax Rate 3",
                "rate": 10
            },
            {
                "id": "dc8a-729f",
                "name": "Tax Rate 5",
                "rate": 10
            },
            {
                "id": "1035-5a9c",
                "name": "Tax Rate 2",
                "rate": 7
            },
            {
                "id": "9f50-9d03",
                "name": "Tax Rate 4",
                "rate": 5
            },
            {
                "id": "d452-f36f",
                "name": "Orange county",
                "rate": 6.5
            },
            {
                "id": "f017-3860",
                "name": "",
                "rate": "5"
            }
        ],
        "hosted_hideCompanyName": true,
        "receipts_emails": [
            "daniel@fattmerchant.com"
        ],
        "merchant_copy_email_receipt_options": {
            "notify_options_all": true
        },
        "hosted_payments_note": "hey fool look at this doof",
        "hosted_payments_success_note": "asdfaklsjdhflkasjhdf",
        "hosted_payments_url_short": "http://bit.ly/2IA0njS",
        "hosted_payments_url_long": "https://qa.fattpay.com/#/pay/demobusiness",
        "grid_view": {
            "item_ids": [
                "cbf9fc4c-22ac-4860-b3d0-8230d5edeeae",
                "e73d9e66-8da5-48e3-983f-e78c220f0d03",
                "af97d681-7ce2-4424-8b17-84fee27a5b91",
                "c17956d9-5688-47f5-bd36-3ab7bb53a4b7",
                "a9ebcecc-d851-4f33-a4d0-24232d961bfd",
                "c2f70e70-4d4c-4327-a03b-42992db8ae98",
                "7d17ce9c-2900-46de-8115-b27451ccfa2e",
                "7a76568b-3b29-4c8f-9217-365fbd06ec64",
                "edf92395-1707-4360-9122-b5ebcf2ebaac",
                "04e39a42-227a-4a57-9f0b-c6d533dd3ec4",
                "131fa816-07b6-4a82-8997-77d95178db7e",
                "0955ffee-3b03-4abc-a255-ef153c52a5f1",
                "bb7175aa-7406-4e5c-8bd9-58f3cfb753ca",
                "bd7534b5-8679-4a7d-8133-60690a79c737",
                "78fc5a2b-740e-422e-af99-8d8c326c5e8d",
                "dddb4ced-b8fd-414e-81fe-85cbacf80803",
                "07dbfb78-e89b-46f6-a27f-c0b139ee3274"
            ],
            "enabled": true
        },
        "vertical": "default",
        "emv_terminal_secret": "password",
        "emv_terminal_id": "3102001",
        "emv_terminal_password": null,
        "emv_password": "Kunal4Team!",
        "emv_user": "Kunal",
        "cart": {
            "enabled": true,
            "keep_after": false
        },
        "branding": "",
        "donations": null,
        "auto_sign_out": {
            "enabled": true,
            "applicable_roles": [
                "sales"
            ]
        },
        "default_invoice_memo": "",
        "send_email_from_user": "true",
        "acknowledgements": {
            "tutorial": true
        },
        "receipt_options": {
            "send_receipts_from_user": true
        },
        "invoice_reminder_interval": "9",
        "invoice_reminder_count": "2",
        "hosted_custom_fields": [
            {
                "id": "7b9e-a565-4bea-e243-0d26",
                "name": "First name",
                "placeholder": "Suneera",
                "required": true,
                "type": "text"
            }
        ],
        "invoice_reminder_enabled": true,
        "tip": {
            "percentages": [
                "10.00",
                "20",
                "25"
            ],
            "smart_tips": {
                "enabled": true,
                "amounts": [
                    "1",
                    "3",
                    "5"
                ]
            },
            "enabled": true
        },
        "social": {
            "facebook_url": "http://www.facebook.com/fattmerchant",
            "linkedin_url": "",
            "twitter_url": "http://www.twitter.com/fattmerchant",
            "website_url": "http://www.fattmerchant.com",
            "instagram_url": "http://instagram.com/fattmerchant"
        },
        "shitty_name": {
            "enabled": false
        },
        "generic_app_name": {
            "enabled": false
        },
        "api_keys": {
            "enabled": true
        },
        "dev_beta_-_ach": {
            "enabled": true
        },
        "default_dashboard": "Sales",
        "advanced_customization": {
            "enabled": true,
            "is_fattmerchant_logo_hidden": false,
            "branding_colors_enabled": false,
            "branding_colors": {
                "color_pop": "#812f91"
            },
            "hosted_custom_fields": [
                {
                    "id": "0098-9afc-d183-cfe0-50f3",
                    "name": "Is this a gift?",
                    "placeholder": "y/n",
                    "required": false,
                    "type": "text"
                },
                {
                    "type": "text",
                    "required": false,
                    "placeholder": "",
                    "name": "Delivery instructions",
                    "id": "0254-bd12-060e-967a-1bbc"
                },
                {
                    "id": "08e4-19d3-3e94-aae0-4807",
                    "name": "color",
                    "placeholder": "",
                    "required": true,
                    "type": "text"
                }
            ]
        },
        "invoice_reference_numbers": {
            "reference_number_enabled": true,
            "reference_number": 3194
        }
    },
    "gateway_type": "TEST",
    "processor": "",
    "partner": null,
    "product_type": "Fattpay Online",
    "is_enterprise": false,
    "is_payfac": false,
    "fm_billing_schedule_id": "a32a68cb-8ad7-46f2-ae2c-a434213554a2",
    "welcome_email_sent_at": null,
    "created_at": "2018-08-01 16:10:16",
    "updated_at": "2020-01-14 18:32:00",
    "deleted_at": null,
    "brand": "fattmerchant",
    "branding": {
        "id": "df3d184d-c8ab-4d23-9f05-e63492ca4518",
        "merchant_id": "a61d78cc-cde9-44ac-8a18-30c39be05879",
        "user_id": "2d089742-6aca-462d-8db3-c2966e1f9e68",
        "name": "",
        "public_url": "https://fattpaydocuments.s3.us-west-2.amazonaws.com/branding/df3d184d-c8ab-4d23-9f05-e63492ca4518.png",
        "tag": "branding",
        "meta": {
            "filesize_bytes": 47202,
            "filesize": "0.47 kB",
            "extension": "png",
            "size": {
                "width": 240,
                "height": 240
            },
            "mime": "image/png"
        },
        "created_at": "2019-08-07 18:44:49",
        "updated_at": "2019-08-07 18:44:50",
        "deleted_at": null
    },
    "allow_ach": true,
    "is_portal": false,
    "allow_credits": true,
    "allow_terminal": true
}
"""
