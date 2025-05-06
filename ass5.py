import re
import random
import string
import time
from datetime import datetime
import tkinter as tk
from tkinter import scrolledtext, Entry, Button, Label, Frame, PhotoImage, END
import nltk
from nltk.stem import WordNetLemmatizer
from nltk.tokenize import word_tokenize
from nltk.corpus import stopwords

# Download necessary NLTK resources (uncomment if needed)
nltk.download('punkt')
nltk.download('wordnet')
nltk.download('stopwords')
nltk.download('punkt_tab')

class NLPProcessor:
    """Natural Language Processing module for the chatbot"""
    
    def __init__(self):
        self.lemmatizer = WordNetLemmatizer()
        self.stop_words = set(stopwords.words('english'))
        
    def preprocess(self, text):
        """Preprocess user input with NLP techniques"""
        # Convert to lowercase
        text = text.lower()
        
        # Remove punctuation
        text = ''.join([char for char in text if char not in string.punctuation])
        
        # Tokenize
        tokens = word_tokenize(text)
        
        # Remove stop words and lemmatize
        processed_tokens = [
            self.lemmatizer.lemmatize(token) 
            for token in tokens 
            if token not in self.stop_words
        ]
        
        return processed_tokens
    
    def extract_entities(self, text):
        """Extract key entities from text"""
        entities = {
            'product': None,
            'issue_type': None,
            'order_number': None,
            'date': None
        }
        
        # Extract order numbers (e.g., ORD-12345 or 12345)
        order_match = re.search(r'(ord[er]*[-\s]?[#:]?\s*)?(\d{5,})', text, re.IGNORECASE)
        if order_match:
            entities['order_number'] = order_match.group(2)
            
        # Extract dates
        date_patterns = [
            r'\d{1,2}[/-]\d{1,2}[/-]\d{2,4}',  # MM/DD/YYYY or similar
            r'(jan|feb|mar|apr|may|jun|jul|aug|sep|oct|nov|dec)[a-z]* \d{1,2}[,]? \d{2,4}'  # Month DD, YYYY
        ]
        
        for pattern in date_patterns:
            date_match = re.search(pattern, text, re.IGNORECASE)
            if date_match:
                entities['date'] = date_match.group(0)
                break
                
        # Extract product mentions
        product_keywords = {
            'laptop': 'laptop', 'computer': 'computer', 'phone': 'smartphone',
            'smartphone': 'smartphone', 'tablet': 'tablet', 'headphone': 'headphones',
            'speaker': 'speaker', 'watch': 'smartwatch', 'tv': 'television',
            'monitor': 'monitor', 'keyboard': 'keyboard', 'mouse': 'mouse',
            'printer': 'printer', 'camera': 'camera'
        }
        
        for word in self.preprocess(text):
            if word in product_keywords:
                entities['product'] = product_keywords[word]
                break
                
        # Extract issue type
        issue_keywords = {
            'broken': 'hardware issue', 'defective': 'hardware issue', 
            'not working': 'hardware issue', 'doesn\'t work': 'hardware issue',
            'damaged': 'hardware issue', 'cracked': 'hardware issue',
            'slow': 'performance issue', 'freezing': 'performance issue', 
            'hang': 'performance issue', 'crash': 'performance issue',
            'refund': 'refund request', 'return': 'return request', 
            'cancel': 'cancel order', 'shipping': 'shipping issue',
            'delivery': 'shipping issue', 'late': 'shipping issue',
            'password': 'account issue', 'login': 'account issue',
            'charge': 'billing issue', 'payment': 'billing issue'
        }
        
        for key in issue_keywords:
            if key in text.lower():
                entities['issue_type'] = issue_keywords[key]
                break
                
        return entities

class ResponseGenerator:
    """Generates appropriate responses based on detected intent and entities"""
    
    def __init__(self):
        # Define response templates for different intents
        self.greeting_responses = [
            "Hello! How can I help you today?",
            "Hi there! I'm the customer service chatbot. What can I assist you with?",
            "Welcome! How may I assist you with our products or services today?"
        ]
        
        self.farewell_responses = [
            "Thank you for chatting with us. Have a great day!",
            "Goodbye! Feel free to contact us again if you need further assistance.",
            "Thank you for your time. Is there anything else I can help you with before you go?"
        ]
        
        self.product_info_responses = [
            "I'd be happy to provide information about our {product}. What would you like to know?",
            "Our {product} comes with a 1-year warranty. What specific details are you looking for?",
            "The {product} is one of our most popular items. Do you have specific questions about it?"
        ]
        
        self.issue_responses = {
            "hardware issue": [
                "I'm sorry to hear about the issue with your {product}. Could you describe the problem in more detail?",
                "For hardware issues with your {product}, we might need to arrange a repair. Can you tell me when you purchased it?"
            ],
            "performance issue": [
                "Performance issues with {product} can often be resolved with updates. Have you tried updating the software?",
                "I understand your {product} is having performance problems. Have you tried restarting it?"
            ],
            "shipping issue": [
                "I apologize for the shipping delay. Let me check the status of your order {order_number}.",
                "Shipping issues can be frustrating. I'll help track your order {order_number} right away."
            ],
            "refund request": [
                "I can help process a refund for your {product}. Do you have your order number handy?",
                "For refund requests, I'll need your order number and the reason for return. Can you provide those details?"
            ],
            "billing issue": [
                "I'm sorry about the billing concern. Could you verify the last 4 digits of the card used for the purchase?",
                "To help with your billing issue, I'll need some details about the transaction. When was the purchase made?"
            ]
        }
        
        self.general_responses = [
            "I'm here to help. Could you provide more details about your question?",
            "I want to make sure I understand correctly. Could you elaborate a bit more?",
            "Thank you for your question. Can you give me some more information so I can help you better?"
        ]
        
        self.not_understood_responses = [
            "I'm sorry, I didn't quite understand that. Could you rephrase your question?",
            "I'm still learning and didn't catch that. Could you try explaining it differently?",
            "Hmm, I'm not sure I follow. Can you provide more details or ask in a different way?"
        ]
        
    def detect_intent(self, tokens):
        """Determine the user's intent based on preprocessed tokens"""
        greeting_words = {'hi', 'hello', 'hey', 'greetings', 'howdy'}
        farewell_words = {'bye', 'goodbye', 'farewell', 'see you', 'later', 'end'}
        product_info_words = {'specs', 'details', 'information', 'tell', 'about', 'features', 'cost', 'price'}
        issue_words = {'problem', 'issue', 'broken', 'not working', 'help', 'fix', 'repair'}
        order_words = {'order', 'purchase', 'bought', 'delivered', 'shipping', 'track'}
        
        tokens_set = set(tokens)
        
        if tokens_set.intersection(greeting_words):
            return "greeting"
        elif tokens_set.intersection(farewell_words):
            return "farewell"
        elif tokens_set.intersection(product_info_words):
            return "product_info"
        elif tokens_set.intersection(issue_words):
            return "issue"
        elif tokens_set.intersection(order_words):
            return "order"
        else:
            return "general"
            
    def generate_response(self, intent, entities):
        """Generate a response based on intent and entities"""
        if intent == "greeting":
            return random.choice(self.greeting_responses)
            
        elif intent == "farewell":
            return random.choice(self.farewell_responses)
            
        elif intent == "product_info":
            if entities['product']:
                template = random.choice(self.product_info_responses)
                return template.format(product=entities['product'])
            else:
                return "Which product would you like information about?"
                
        elif intent == "issue":
            if entities['issue_type'] and entities['issue_type'] in self.issue_responses:
                template = random.choice(self.issue_responses[entities['issue_type']])
                response = template.format(
                    product=entities['product'] if entities['product'] else "product",
                    order_number=entities['order_number'] if entities['order_number'] else "[order number]"
                )
                return response
            elif entities['product']:
                return f"I understand you're having an issue with your {entities['product']}. Could you describe the problem?"
            else:
                return "I'd like to help with your issue. Could you please provide more details about the problem and which product it concerns?"
                
        elif intent == "order":
            if entities['order_number']:
                return f"Let me look up information for order number {entities['order_number']}. Please wait a moment..."
            else:
                return "I can help with order-related questions. Do you have your order number handy?"
                
        else:  # general or not understood
            if any(entities.values()):  # If we extracted any entities
                return f"I see you're asking about {entities['product'] if entities['product'] else 'a product'}. How can I help with that?"
            else:
                return random.choice(self.general_responses)

class CustomerServiceChatbot:
    """Main chatbot class that integrates NLP processing and response generation"""
    
    def __init__(self):
        self.nlp = NLPProcessor()
        self.response_gen = ResponseGenerator()
        self.conversation_history = []
        self.user_name = None
        self.context = {}
        
    def process_message(self, user_input):
        """Process user input and generate a response"""
        # Skip processing for empty messages
        if not user_input.strip():
            return "Is there something I can help you with?"
            
        # Store the message in conversation history
        self.conversation_history.append(("user", user_input))
        
        # Process the message with NLP
        tokens = self.nlp.preprocess(user_input)
        entities = self.nlp.extract_entities(user_input)
        
        # Update context with extracted entities
        for key, value in entities.items():
            if value:
                self.context[key] = value
                
        # Extract user name if not already known
        if not self.user_name:
            name_match = re.search(r'my name is (\w+)', user_input.lower())
            if name_match:
                self.user_name = name_match.group(1).capitalize()
                greeting = f"Nice to meet you, {self.user_name}! "
            else:
                greeting = ""
        else:
            greeting = ""
            
        # Generate a response
        intent = self.response_gen.detect_intent(tokens)
        response = self.response_gen.generate_response(intent, entities)
        
        # Add personalization if we know the user's name
        if self.user_name and random.random() < 0.3:  # 30% chance to use name
            if "." in response:
                parts = response.split(".", 1)
                response = parts[0] + f", {self.user_name}." + parts[1]
            else:
                response = response.rstrip("?!.,") + f", {self.user_name}."
                
        # Store the response in conversation history
        self.conversation_history.append(("bot", response))
        
        # Simulate typing delay based on response length
        typing_delay = min(len(response) * 0.01, 1.5)  # Cap at 1.5 seconds
        
        return greeting + response, typing_delay

class ChatbotGUI:
    """Graphical User Interface for the chatbot"""
    
    def __init__(self, root):
        self.root = root
        self.chatbot = CustomerServiceChatbot()
        self.setup_ui()
        
    def setup_ui(self):
        """Set up the user interface components"""
        self.root.title("Customer Service Chatbot")
        self.root.geometry("500x600")
        self.root.configure(bg="#f0f0f0")
        
        # Create header
        header_frame = Frame(self.root, bg="#4a7abc", height=60)
        header_frame.pack(fill="x")
        
        # Company name/logo
        Label(header_frame, text="TechSupport Assistant", 
              font=("Arial", 16, "bold"), bg="#4a7abc", fg="white").pack(pady=10)
        
        # Chat display area
        self.chat_frame = Frame(self.root, bg="#f0f0f0")
        self.chat_frame.pack(fill="both", expand=True, padx=10, pady=10)
        
        self.chat_display = scrolledtext.ScrolledText(self.chat_frame, wrap=tk.WORD, 
                                                     bg="white", font=("Arial", 11))
        self.chat_display.pack(fill="both", expand=True)
        self.chat_display.config(state=tk.DISABLED)
        
        # Input area
        input_frame = Frame(self.root, bg="#f0f0f0", height=100)
        input_frame.pack(fill="x", padx=10, pady=10)
        
        self.user_input = Entry(input_frame, font=("Arial", 12), bd=1, relief=tk.SOLID)
        self.user_input.pack(side=tk.LEFT, fill="x", expand=True, ipady=5)
        self.user_input.bind("<Return>", self.send_message)
        
        send_button = Button(input_frame, text="Send", font=("Arial", 11), bg="#4a7abc", fg="white",
                             relief=tk.FLAT, command=self.send_message)
        send_button.pack(side=tk.RIGHT, padx=10, ipadx=10, ipady=5)
        
        # Initial greeting
        self.display_bot_message("Hello! I'm the TechSupport Assistant. How can I help you today?")
        
    def send_message(self, event=None):
        """Handle sending user messages"""
        user_message = self.user_input.get()
        if not user_message.strip():
            return
            
        # Clear input field
        self.user_input.delete(0, END)
        
        # Display user message
        self.display_user_message(user_message)
        
        # Process message and get response
        response, delay = self.chatbot.process_message(user_message)
        
        # Simulate typing
        self.root.after(int(delay * 1000), lambda: self.display_bot_message(response))
        
    def display_user_message(self, message):
        """Display user message in the chat"""
        self.chat_display.config(state=tk.NORMAL)
        if self.chat_display.index('end-1c') != '1.0':
            self.chat_display.insert(tk.END, "\n\n")
        
        self.chat_display.insert(tk.END, "You: ", "user_tag")
        self.chat_display.insert(tk.END, message, "user_message")
        
        self.chat_display.tag_config("user_tag", foreground="#4a7abc", font=("Arial", 11, "bold"))
        self.chat_display.tag_config("user_message", foreground="black", font=("Arial", 11))
        
        self.chat_display.config(state=tk.DISABLED)
        self.chat_display.see(tk.END)
        
    def display_bot_message(self, message):
        """Display bot message in the chat"""
        self.chat_display.config(state=tk.NORMAL)
        if self.chat_display.index('end-1c') != '1.0':
            self.chat_display.insert(tk.END, "\n\n")
            
        self.chat_display.insert(tk.END, "Bot: ", "bot_tag")
        self.chat_display.insert(tk.END, message, "bot_message")
        
        self.chat_display.tag_config("bot_tag", foreground="#c41a7b", font=("Arial", 11, "bold"))
        self.chat_display.tag_config("bot_message", foreground="black", font=("Arial", 11))
        
        self.chat_display.config(state=tk.DISABLED)
        self.chat_display.see(tk.END)

def main():
    root = tk.Tk()
    app = ChatbotGUI(root)
    root.mainloop()

if __name__ == "__main__":
    main()