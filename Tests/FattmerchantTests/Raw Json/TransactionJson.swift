//
//  TransactionJson.swift
//  FattmerchantTests
//
//  Created by Tulio Troncoso on 1/22/20.
//  Copyright © 2020 Fattmerchant. All rights reserved.
//

let transactionjson =
"""
{
    "id": "b903ca0a-03dc-4ebb-b592-8251900fa9b3",
    "invoice_id": "24e31da2-a542-40b8-bfdc-6e0664a53108",
    "reference_id": "7fc7b78d-4ccb-4658-855a-a2657d1ad2de",
    "recurring_transaction_id": "",
    "auth_id": null,
    "type": "refund",
    "source": "Android|CPSDK|NMI",
    "channel": "ios",
    "source_ip": "45.27.255.121",
    "is_merchant_present": true,
    "merchant_id": "a61d78cc-cde9-44ac-8a18-30c39be05879",
    "user_id": "30c6eeb6-64b6-47f6-bcf6-787a9c58798b",
    "customer_id": "1815233a-9893-4e61-a166-316c5c40246e",
    "payment_method_id": "cbbfa0a2-b38a-4143-9357-a30f782e7b8b",
    "is_manual": false,
    "success": true,
    "message": "",
    "meta": [],
    "total": 0.09,
    "method": "card",
    "pre_auth": false,
    "is_captured": 0,
    "last_four": "5544",
    "interchange_code": "",
    "interchange_fee": null,
    "batch_id": "",
    "batched_at": "2020-01-09 19:46:41",
    "emv_response": "",
    "avs_response": "",
    "cvv_response": "",
    "pos_entry": "",
    "pos_salesperson": "",
    "receipt_email_at": null,
    "receipt_sms_at": null,
    "settled_at": null,
    "created_at": "2020-01-09 19:46:41",
    "updated_at": "2020-01-09 19:46:41",
    "gateway_id": null,
    "issuer_auth_code": null,
    "total_refunded": 0,
    "is_refundable": false,
    "is_voided": false,
    "is_voidable": false,
    "schedule_id": null,
    "child_captures": [],
    "parent_auth": null,
    "gateway_name": "",
    "response": null,
    "is_settling": null,
    "customer": {
        "id": "1815233a-9893-4e61-a166-316c5c40246e",
        "firstname": "SWIPE",
        "lastname": "CUSTOMER",
        "company": "",
        "email": "",
        "cc_emails": null,
        "cc_sms": null,
        "phone": "",
        "address_1": "",
        "address_2": "",
        "address_city": "",
        "address_state": "",
        "address_zip": "",
        "address_country": "",
        "notes": null,
        "reference": "",
        "options": null,
        "created_at": "2020-01-09 19:37:34",
        "updated_at": "2020-01-09 19:37:34",
        "deleted_at": null,
        "allow_invoice_credit_card_payments": true,
        "gravatar": false
    },
    "child_transactions": [],
    "files": [],
    "payment_method": {
        "id": "cbbfa0a2-b38a-4143-9357-a30f782e7b8b",
        "customer_id": "1815233a-9893-4e61-a166-316c5c40246e",
        "merchant_id": "eb48ef99-aa78-496e-9b01-421f8daf7323",
        "user_id": "30c6eeb6-64b6-47f6-bcf6-787a9c58798b",
        "nickname": "MASTERCARD: SWIPE CUSTOMER (ending in: 5544)",
        "has_cvv": 0,
        "is_default": 0,
        "method": "card",
        "person_name": "SWIPE CUSTOMER",
        "card_type": "mastercard",
        "card_last_four": "5544",
        "card_exp": null,
        "bank_name": null,
        "bank_type": null,
        "bank_holder_type": null,
        "address_1": null,
        "address_2": null,
        "address_city": null,
        "address_state": null,
        "address_zip": null,
        "address_country": null,
        "purged_at": null,
        "deleted_at": null,
        "created_at": "2020-01-09 19:37:35",
        "updated_at": "2020-01-09 19:37:35",
        "meta": null,
        "bin_type": null,
        "card_exp_datetime": [],
        "is_usable_in_vt": false,
        "is_tokenized": false,
        "customer": {
            "id": "1815233a-9893-4e61-a166-316c5c40246e",
            "firstname": "SWIPE",
            "lastname": "CUSTOMER",
            "company": "",
            "email": "",
            "cc_emails": null,
            "cc_sms": null,
            "phone": "",
            "address_1": "",
            "address_2": "",
            "address_city": "",
            "address_state": "",
            "address_zip": "",
            "address_country": "",
            "notes": null,
            "reference": "",
            "options": null,
            "created_at": "2020-01-09 19:37:34",
            "updated_at": "2020-01-09 19:37:34",
            "deleted_at": null,
            "allow_invoice_credit_card_payments": true,
            "gravatar": false
        }
    },
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
            "godviewOmniSearchTooltip": true,
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
    "merchant": {
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
                "reference_number": 3208
            }
        },
        "notes": "",
        "gateway_type": "TEST",
        "vendor_keys": {
            "spreedly": {
                "key": "GhezbsYOwD69bFSQneOm1y4mxOX",
                "gateway": "Vk6quWz75Qe7ysHZWvua8laICjn",
                "created_at": "2016-10-07 16:52:45",
                "credentials": [],
                "details": {
                    "token": "Vk6quWz75Qe7ysHZWvua8laICjn",
                    "gateway_type": "test",
                    "name": "Spreedly Test",
                    "description": null,
                    "characteristics": {
                        "supports_purchase": "true",
                        "supports_authorize": "true",
                        "supports_capture": "true",
                        "supports_credit": "true",
                        "supports_general_credit": "true",
                        "supports_void": "true",
                        "supports_verify": "true",
                        "supports_reference_purchase": "true",
                        "supports_purchase_via_preauthorization": "true",
                        "supports_offsite_purchase": "true",
                        "supports_offsite_authorize": "true",
                        "supports_3dsecure_purchase": "true",
                        "supports_3dsecure_authorize": "true",
                        "supports_store": "true",
                        "supports_remove": "true",
                        "supports_fraud_review": "false",
                        "supports_disburse": "true",
                        "supports_reference_authorization": "true"
                    },
                    "credentials": null,
                    "gateway_specific_fields": null,
                    "payment_methods": {
                        "payment_method": [
                            "credit_card",
                            "sprel",
                            "third_party_token",
                            "bank_account",
                            "apple_pay",
                            "android_pay"
                        ]
                    },
                    "state": "retained",
                    "redacted": "false",
                    "created_at": "2016-10-07T16:55:46Z",
                    "updated_at": "2016-10-07T16:55:46Z"
                }
            },
            "terminals": [
                {
                    "id": "dde9-0cc5",
                    "nickname": "Front Desk",
                    "key": "zsSrFdi8Nf",
                    "status_port": "3594291",
                    "serial": "",
                    "type": "dejavoo",
                    "proxy": "http://spinpos.net:80/spin/cgi.html",
                    "model": "Z8",
                    "isDefault": true
                },
                {
                    "id": "f791-aa32",
                    "nickname": "Front Desk 2",
                    "key": "zsSrFdi8Nf",
                    "status_port": "3594291",
                    "serial": "",
                    "type": "dejavoo",
                    "proxy": "http://spinpos.net:80/spin/cgi.html",
                    "isDefault": false
                }
            ]
        },
        "processor": "",
        "partner": null,
        "product_type": "Fattpay Online",
        "is_enterprise": false,
        "is_payfac": false,
        "fm_billing_schedule_id": "a32a68cb-8ad7-46f2-ae2c-a434213554a2",
        "welcome_email_sent_at": null,
        "created_at": "2018-08-01 16:10:16",
        "updated_at": "2020-01-21 18:02:00",
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
}
"""
