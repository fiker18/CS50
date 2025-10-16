-- Keep a log of any SQL queries you execute as you solve the mystery.

-- First thing I did is based on the given data to check the description from the crime_scene_report table
-- so that I can get better understanding of the situation.
SELECT description FROM crime_scene_report
WHERE
    year = 2024 AND month = 07 AND day = 28 AND street = 'Humphrey Street';
-- From this query I learned that the time the crime scene took was 10:30 Am,
-- it is related to a Bakery and
-- there where three witnesses.

-- With information then I checked the transcript from those three witnesses.
SELECT transcript FROM interviews WHERE year = 2024 AND month = 07 AND day = 28;
-- From this query I learned that the thief left the baking garage within 10 minuites with his car,
-- the thief withdrew money form an ATM located at Legget Street and
-- the theif took a phone call with his accomplice that took less than a minute where the disccused about booking an earliest flight in the next day.

-- this query will give the name of the thief based on all information gathered.
SELECT name FROM people
WHERE
-- Here I want to get an account_number of all people who withdrew money from the ATM at the same day as the thief and then using their account number to get a list of person_id.
    ID IN (SELECT person_id FROM bank_accounts
    WHERE account_number IN (SELECT account_number FROM atm_transactions
        WHERE year = 2024 AND month = 07 AND day = 28 AND atm_location = 'Leggett Street' AND transaction_type = 'withdraw'))

    AND
-- Here I want to get a list of the phone numbers where a call lasted less than a minute at the exact date the thief called his accomplice.
    phone_number IN (SELECT caller FROM phone_calls
    WHERE year = 2024 AND month = 07 AND day = 28 AND duration < 60)

    AND
-- Here I want to get the passport number of the peoples who travelled the next day on the earliest flight from Fiftyville.
    passport_number IN (SELECT passport_number FROM passengers
    WHERE flight_id = (SELECT id FROM flights
        WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
        AND year = 2024 AND month = 07 AND day = 29 ORDER BY hour ASC LIMIT 1))

    AND
-- Here I want to get the license plate of all cars who exited the bakery within 10 miniutes from when the crime happened.
    license_plate IN (SELECT license_plate FROM bakery_security_logs
    WHERE year = 2024 AND month = 07 AND day = 28 AND hour = 10 AND minute >= 15 AND minute <= 25 AND activity = 'exit');


-- In this query I want to find the name of the airport where the first flight from Fiftyville in the next day went.
SELECT city FROM airports
WHERE
    id = (SELECT destination_airport_id FROM flights
    WHERE origin_airport_id = (SELECT id FROM airports WHERE city = 'Fiftyville')
    AND year = 2024 AND month = 07 AND day = 29 ORDER BY hour ASC LIMIT 1);

-- In this query I want to find the name of the accomplice by checking the phone calls where the theif called to get his flight ticket.
SELECT name FROM People
WHERE phone_number = (SELECT receiver FROM phone_calls
    WHERE caller = (SELECT phone_number FROM people WHERE name = 'Bruce')
    AND year = 2024 AND month = 07 AND day = 28 AND duration < 60);

