import argparse
from typing import *
import json
from dacite import from_dict
from dataclasses import dataclass, field, asdict
import logging
from jinja2 import Template
import os

logger = logging.getLogger("CONFIG")

class ColorFormatter(logging.Formatter):
    def format(self, record):
        """Render log records with ANSI colors based on severity level."""
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
    command: Optional[str] = None

def extract_options(profiles: dict[str, Profile], profileName: str) -> Dict[str, int]:
    """
    Resolve inherited profile options into a flat flag-to-bool mapping.

    :param profiles: A dictionary of profile names to Profile objects.
    :param profileName: The name of the profile to resolve.
    :return: A dictionary mapping option keys to 0/1 values.
    """
    options: Dict[str, int] = {}

    for parent in profiles[profileName].inherit:
        options.update(extract_options(profiles, parent))

    for option in profiles[profileName].options:
        logger.debug(f"Processing option: {option}")
        if option.endswith("=0"):
            options[option[:-2]] = 0
        elif option.endswith("=1"):
            options[option[:-2]] = 1

    return options

def make_cmake_options(options: Dict[str, int]) -> str:
    """
    Convert profile options to CMake -D KEY=ON/OFF arguments.

    :param options: A dictionary mapping option keys to 0/1 values.
    :return: A string of CMake options, e.g. "-DNTT_ENGINE=ON -DNTT_EDITOR=OFF".
    """
    return " ".join([f"-D{key}={'ON' if value == 1 else 'OFF'}" for key, value in options.items()])

def main():
    parser = argparse.ArgumentParser(description="NTT Graphics Engine Configuration")
    parser.add_argument("-p", "--profile", type=str, default="default", help="Profile to use")
    parser.add_argument("-v", "--verbose", action="store_true", help="Enable verbose logging")

    args = parser.parse_args()

    if args.verbose:
        logger.setLevel(logging.DEBUG)

    profiles: dict[str, Profile] = {}

    usedProfile = "engine:debug"

    with open("profiles.json", "r") as f:
        config = json.load(f)
        logger.debug(f"\nLoaded config: {json.dumps(config, indent=4)}")

        for profileName in config:
            profiles[profileName] = from_dict(Profile, config[profileName])

    if args.profile == "default":
        usedProfile = "engine:debug"
    elif args.profile not in profiles:
        logger.warning(f"Profile \"{args.profile}\" not found, using default profile")
        usedProfile = "engine:debug"
    else:
        usedProfile = args.profile

    logger.info(f"Using profile: {usedProfile}")

    options = extract_options(profiles, usedProfile)

    logger.debug(f"\n--------- Options for profile {usedProfile} -----------\n{json.dumps(options, indent=4)}")

    cmake_options = make_cmake_options(options)
    logger.debug(f"CMake options: {cmake_options}")

    if profiles[usedProfile].command is None:
        logger.error("Custom command is not supported yet, cannot execute it")
        return

    command = profiles[usedProfile].command
    template = Template(command)
    rendered_command = template.render(options=cmake_options) + " -Wno-dev"
    logger.info(f"Executing command: {rendered_command}")
    os.system(rendered_command)

if __name__ == "__main__":
    main()
