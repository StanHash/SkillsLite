
# SkillsLite

The goal here is to provide an alternative to the [FEU Skill System](https://github.com/FireEmblemUniverse/SkillSystem_FE8) that is **minimalistic** with a focus on **quality** and **stability**, as well as hopefully providing better documentation as to how everything works.

*SkillsLite is not meant as a replacement to the "original" Skill System*. SkillsLite will never reach the amount of features it provides, and for that reason it is unlikely that it will ever be overshadowed. As I said, SkillsLite is an *alternative*.

## About the build system

The build system for the project as a whole is a classic EA buildfile (that is, featuring a MakeHack.cmd). Classic buildfiles are more approachable to newbies and people are more familiar with it in general, which is why I believe it is a better option for the demonstration.

However, for wizardry only, a makefile is also provided. It provides a convenient way for high end users to build the wizardry components of the project. But using it entirely optional, as, of course, contributors can still build their asm using classic methods just fine. Their contributions will be accepted as long as coding and organizational conventions are respected.

See [SETUP.md](SETUP.md) for detailed instructions on how to set up for both the standard buildfile and the wizardry makefile.

## About the implementation

The SkillsLite core skill system will *replace support data with skills* within the unit struct. Of course, supports won't be removed. Instead, support-related data will be moved to be alongside BWL stats, which is where skills reside in circles'/FEU's skill system.

I believe this will result in a cleaner overall implementation:

- All units will have all 6 of their skills saved. This means that one unit can learn all 6 skills dynamically, and generics will be able to have their skillsets modified like for any other units.
- The skill list being in the unit struct means that it doesn't need to be recomputed for each time it is required (one can just look it up). This should reduce lag when it comes to battle computations.
- Since the skill list lies inside the unit struct, it also lies inside the battle unit struct, which will make implementing skills such as nihil much easier (it would just clear the skill list from the opponent's battle struct).
- Other things, probably.

The one disadvantage is that supports will need to be reworked to fit the new allocations. This in turn means that other support rework hacks will be inevitably incompatible with SkillsLite, unfortunately.

## Target feature list for 1.0

This is the tentative list of things that needs to be done before the 1.0 release. This is subject to change.

### Core features

- ✓ Essential fixes
- Basic QoL hacks
- Core skill system
  - ✓ Replace support data with skills, move supports to BWL
  - Skill Learning on ✓ Load, ✓ Level Up, Promotion
- Some skills (see below)
- Supporting hacks for everything else
- Stat screen that is actually modular

### Skill list

| Name    | Effect
| ------- | ------
| Astra   | Hit 5 times for half damage. PROC: Skill/2%
| Luna    | Hit ignores defense. PROC: Skill%
| Sol     | Heal damage dealt. PROC: Skill%
| Pavise  | Recieve no damage. PROC: Skill%
| Vantage | Always hit first
| ✓ Wrath | Crit when attacked if Hp < 50%
| Miracle | Always survive at min 1 Hp when Hp starts full
| Counter | Adjacent opponent takes recoil from physical hits
| Nihil   | Negate opponent's battle skills
| ✓ Charm | +2/-2 damage dealt/recieved for allies within 2 tiles
| Canto   | Allows unit to move again after certain actions
| Paragon | Doubles exp gain
| Renewal | Heal 5 hp at start of turn
| Savior  | Negates stat penalties from rescuing
| Pass    | Can move through enemies
| Provoke | AI will prioritize targetting this unit
| Shove   | Command: Pushes an adjacent ally one tile away
| Steal   | Command: Steal item from adjacent enemy

### Future plans (for 2.0 and above)

- Skill scrolls
- Skill activations messages during battle animations (would require a full battle system rework to be implemented cleanly)
- Charm (and other potential "aura" skill) preview on battle preview (display charm icon + color above unit with charm if it affects one of the battlers)
