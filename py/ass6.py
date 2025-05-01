def airline_cargo_expert_system():
    print("Airline & Cargo Expert System: Welcome! How can I assist you with scheduling today?")

    while True:
        user_input = input("You: ").strip().lower()

        if user_input == "hi" or user_input == "hello":
            print("Airline & Cargo Expert System: Hello! How can I help you with your scheduling needs?")
        elif user_input == "thanks" or user_input == "thank you":
            print("Airline & Cargo Expert System: You're welcome! Fly safe and have a great day!")
        elif user_input == "sorry":
            print("Airline & Cargo Expert System: No problem at all!")
        elif "no" in user_input or "don't" in user_input or "not interested" in user_input:
            print("Airline & Cargo Expert System: I understand. Let me know if you need any assistance later!")
        elif user_input == "bye" or user_input == "exit":
            print("Airline & Cargo Expert System: Thank you for using our services. Safe travels!")
            break

        elif ("book" in user_input or "ticket" in user_input or "flight" in user_input) and not ("cargo" in user_input or "freight" in user_input):
            departure = input("Airline & Cargo Expert System: Please enter your departure city: ").strip().lower()
            destination = input("Airline & Cargo Expert System: Please enter your destination city: ").strip().lower()
           
            route_options = {
                "new york-london": ["Delta Airlines: 7:30 AM, 6:00 PM", "British Airways: 9:00 AM, 11:00 PM"],
                "new york-paris": ["Air France: 10:15 AM", "American Airlines: 8:45 PM"],
                "chicago-tokyo": ["United Airlines: 1:20 PM", "JAL: 11:45 AM"],
                "los angeles-sydney": ["Qantas: 10:30 PM", "United Airlines: 11:15 PM"],
                "london-dubai": ["Emirates: 8:20 AM, 3:15 PM", "British Airways: 10:30 AM"]
            }
           
            route_key = f"{departure}-{destination}"
            alt_route_key = f"{destination}-{departure}"
           
            if route_key in route_options:
                print(f"Airline & Cargo Expert System: Available flights from {departure.title()} to {destination.title()}:")
                for flight in route_options[route_key]:
                    print(f"- {flight}")
               
                class_choice = input("Airline & Cargo Expert System: What class would you prefer? (Economy/Business/First): ").strip().lower()
                passengers = input("Airline & Cargo Expert System: How many passengers will be traveling?: ").strip()
               
                print(f"Airline & Cargo Expert System: Thank you! I've noted your preference for {class_choice.title()} class for {passengers} passenger(s).")
                print("Airline & Cargo Expert System: Please contact our booking department at 1-800-FLY-BOOK to complete your reservation.")
           
            elif alt_route_key in route_options:
                print(f"Airline & Cargo Expert System: I found return flights from {destination.title()} to {departure.title()}. Would you like to see these instead?")
            else:
                print(f"Airline & Cargo Expert System: I'm sorry, we don't have direct flights between {departure.title()} and {destination.title()} in our database.")
                print("Airline & Cargo Expert System: Please contact our customer service at 1-800-FLY-HELP for assistance with custom routes.")

        elif "cargo" in user_input or "freight" in user_input or "shipment" in user_input:
            origin = input("Airline & Cargo Expert System: Please enter the origin city for your cargo: ").strip().lower()
            destination = input("Airline & Cargo Expert System: Please enter the destination city: ").strip().lower()
           
            cargo_type = input("Airline & Cargo Expert System: What type of cargo are you shipping? (General/Perishable/Hazardous/Oversized): ").strip().lower()
            cargo_weight = input("Airline & Cargo Expert System: Approximate weight of cargo (in kg): ").strip()
           
            try:
                weight = float(cargo_weight)
                cargo_options = {
                    "perishable": ["Refrigerated containers required", "Priority shipping available", "Temperature monitoring service"],
                    "hazardous": ["Special handling required", "Safety documentation necessary", "Restricted flight options"],
                    "oversized": ["Special loading equipment needed", "Limited flight availability", "Advance booking required"],
                    "general": ["Standard shipping options available", "Regular flight schedules apply"]
                }
               
                if cargo_type in cargo_options:
                    print(f"Airline & Cargo Expert System: Important notes for {cargo_type} cargo:")
                    for note in cargo_options[cargo_type]:
                        print(f"- {note}")
               
                # Calculate estimated price based on weight and cargo type
                base_rate = 5.0  # Base rate per kg in USD
                multipliers = {"general": 1.0, "perishable": 1.5, "hazardous": 2.0, "oversized": 1.8}
               
                estimated_cost = weight * base_rate * multipliers.get(cargo_type, 1.0)
                print(f"Airline & Cargo Expert System: Estimated shipping cost: ${estimated_cost:.2f}")
                print("Airline & Cargo Expert System: For final quotes and booking, please contact our cargo department at 1-800-AIR-CARGO.")
           
            except ValueError:
                print("Airline & Cargo Expert System: Invalid weight input. Please enter a numeric value.")

        elif "delay" in user_input or "cancel" in user_input or "postpone" in user_input:
            flight_number = input("Airline & Cargo Expert System: Please enter your flight number: ").strip().upper()
           
            if flight_number:
                # This would typically check a database - we're simulating responses
                if "delay" in user_input:
                    print(f"Airline & Cargo Expert System: Flight {flight_number} status check:")
                    import random
                    delay_time = random.choice([0, 30, 45, 60, 120])
                   
                    if delay_time == 0:
                        print(f"Flight {flight_number} is currently on schedule.")
                    else:
                        print(f"Flight {flight_number} is currently delayed by {delay_time} minutes.")
                        print("Compensation may be available for delays over 3 hours. Please check with customer service.")
               
                elif "cancel" in user_input:
                    print(f"Airline & Cargo Expert System: For cancellation of flight {flight_number}:")
                    print("- Economy tickets: 70% refund if cancelled 24+ hours before departure")
                    print("- Business/First class: 85% refund if cancelled 24+ hours before departure")
                    print("- All classes: 50% refund if cancelled less than 24 hours before departure")
                    print("Please contact our customer service at 1-800-FLY-HELP to process your cancellation.")
            else:
                print("Airline & Cargo Expert System: Please provide a valid flight number to check status or make changes.")

        elif "schedule" in user_input or "reschedule" in user_input:
            print("Airline & Cargo Expert System: For scheduling or rescheduling, we need some information:")
            current_booking = input("Do you have an existing booking? (yes/no): ").strip().lower()
           
            if current_booking == "yes":
                booking_ref = input("Please enter your booking reference: ").strip().upper()
                print(f"Airline & Cargo Expert System: Retrieving booking {booking_ref}...")
                print("Airline & Cargo Expert System: For security reasons, please contact our scheduling department at 1-800-FLY-SCHED with your booking reference to make changes.")
            else:
                print("Airline & Cargo Expert System: To create a new schedule, please use our booking service.")
                print("Would you like me to help you with flight booking or cargo shipping?")

        elif "baggage" in user_input or "luggage" in user_input:
            flight_class = input("Airline & Cargo Expert System: What is your ticket class? (Economy/Business/First): ").strip().lower()
           
            baggage_allowance = {
                "economy": ["1 checked bag (23kg max)", "1 carry-on (7kg max)", "1 personal item"],
                "business": ["2 checked bags (32kg max each)", "1 carry-on (10kg max)", "1 personal item"],
                "first": ["3 checked bags (32kg max each)", "1 carry-on (10kg max)", "1 personal item"]
            }
           
            if flight_class in baggage_allowance:
                print(f"Airline & Cargo Expert System: Baggage allowance for {flight_class.title()} class:")
                for item in baggage_allowance[flight_class]:
                    print(f"- {item}")
               
                excess = input("Do you need information about excess baggage charges? (yes/no): ").strip().lower()
                if excess == "yes":
                    print("Airline & Cargo Expert System: Excess baggage is charged at $25 per kg over the allowance.")
                    print("Pre-booking excess baggage online saves 20% on these charges.")
            else:
                print("Airline & Cargo Expert System: Please specify a valid ticket class (Economy/Business/First).")

        elif "track" in user_input or "status" in user_input:
            tracking_type = input("Airline & Cargo Expert System: Would you like to track a passenger flight or cargo shipment? ").strip().lower()
           
            if "passenger" in tracking_type or "flight" in tracking_type:
                flight_number = input("Airline & Cargo Expert System: Please enter the flight number: ").strip().upper()
                print(f"Airline & Cargo Expert System: Flight {flight_number} is currently in transit.")
                print("For real-time updates, please visit our website or use our mobile app.")
           
            elif "cargo" in tracking_type or "shipment" in tracking_type:
                tracking_id = input("Airline & Cargo Expert System: Please enter your tracking ID: ").strip().upper()
                print(f"Airline & Cargo Expert System: Tracking shipment {tracking_id}...")
                print("Your shipment is currently being processed at our sorting facility.")
                print("Estimated delivery is within the next 48 hours.")
                print("For detailed tracking, please visit our cargo tracking portal online.")
           
            else:
                print("Airline & Cargo Expert System: I'm not sure what you'd like to track. Please specify 'passenger flight' or 'cargo shipment'.")

        else:
            print("Airline & Cargo Expert System: I'm not sure I understand your request. For immediate assistance, please call:")
            print("- Passenger flights: 1-800-FLY-HELP")
            print("- Cargo shipping: 1-800-AIR-CARGO")

# Run the expert system
airline_cargo_expert_system()