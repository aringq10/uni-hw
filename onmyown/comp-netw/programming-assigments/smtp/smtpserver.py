from aiosmtpd.controller import Controller
import os
import asyncio

class FileHandler:
    async def handle_DATA(self, server, session, envelope):
        os.makedirs("emails", exist_ok=True)
        count = len(os.listdir("emails")) + 1
        with open(f"emails/email_{count}.txt", "w") as f:
            f.write(envelope.content.decode('utf8', errors='replace'))
        print(f"Saved email {count}")
        return '250 Message accepted for delivery'

# Create and start the controller
controller = Controller(FileHandler(), hostname='localhost', port=1025)
controller.start()
print("SMTP server running. Press Ctrl+C to stop")

# Keep the program running
try:
    asyncio.get_event_loop().run_forever()
except KeyboardInterrupt:
    print("Server stopped")
    controller.stop()
