import argparse
from typing import *
import json
from dacite import from_dict
from dataclasses import dataclass, field, asdict
import logging

logger = logging.getLogger("CONFIG")

class ColorFormatter(logging.Formatter):
    def format(self, record):
        start_color = ""
        end_color = "\033[0m"
        if record.levelno == logging.DEBUG:
            start_color = "\033[94m"  # Blue
        elif record.levelno == logging.INFO:
            start_color = "\033[92m"  # Green
        elif record.levelno == logging.WARNING:
            start_color = "\033[93m"  # Yellow
        elif record.levelno == logging.ERROR:
            start_color = "\033[91m"  # Red
        record.msg = f"{start_color}{record.msg}{end_color}"
        return f"{start_color}{super().format(record)}{end_color}"

handler = logging.StreamHandler()
handler.setFormatter(ColorFormatter("[%(name)-10s] [%(levelname)-7s] %(message)s"))

# set color for logging
logger.addHandler(handler)
logger.setLevel(logging.INFO)

@dataclass
class Profile:
    inherit: List[str] = field(default_factory=list)
    options: List[str] = field(default_factory=list)

def extract_options(profiles: dict[str, Profile], profileName: str) -> Dict[str, int]:
    options: Dict[str, int] = {}

    for parent in profiles[profileName].inherit:
        options.update(extract_options(profiles, parent))

    for option in profiles[profileName].options:
        if option.endswith("=0"):
            options[option[:-2]] = 0
        elif option.endswith("=1"):
            options[option[:-2]] = 1

    return options

def main():
    parser = argparse.ArgumentParser(description="NTT Graphics Engine Configuration")
    parser.add_argument("-p", "--profile", type=str, default="default", help="Profile to use")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose logging")

    args = parser.parse_args()

    if args.verbose:
        logger.setLevel(logging.DEBUG)

    profiles: dict[str, Profile] = {}

    with open("config.json", "r") as f:
        config = json.load(f)
        logger.debug(f"\nLoaded config: {json.dumps(config, indent=4)}")

        for profileName in config:
            profiles[profileName] = from_dict(Profile, config[profileName])

    if args.profile == "default":
        profileName = "engine:debug"
    elif args.profile not in profiles:
        # change color to green
        logger.warning(f"Profile \"{args.profile}\" not found, using default profile")
        profileName = "engine:debug"

    logger.info(f"Using profile: {profileName}")

    options = extract_options(profiles, profileName)

    logger.debug(f"\n--------- Options for profile {profileName} -----------\n{json.dumps(options, indent=4)}")


if __name__ == "__main__":
    main()
